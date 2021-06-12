#include <Arduino.h>
// Select you T-Watch in the platformio.ini file
#include <LilyGoWatch.h>
#include <WiFi.h>

#include "backlightController.hpp"
#include "noEventsMonitor.hpp"
#include "touchScreenMonitor.hpp"
#include "freertos.hpp"
#include "task.hpp"

// C++ object which will allow access to the functions of the Watch
TTGOClass *watch;

void showClock(void *pvParameters)
{
    while (true)
    {
        auto tnow = watch->rtc->formatDateTime(PCF_TIMEFORMAT_HMS);
        watch->tft->setCursor(8, 8);
        watch->tft->setTextSize(3);
        watch->tft->setTextFont(2);
        watch->tft->print(tnow);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void setBrightness(uint8_t level)
{
    watch->setBrightness(level);
}

bool getTouched()
{
    return watch->touch->getTouched();
}

void sleep()
{
    //setCpuFrequencyMhz(20);
    delay(100);
    gpio_wakeup_enable((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    Serial.println("before light sleep");
    esp_err_t lightSleepResult = esp_light_sleep_start();

    Serial.printf("light sleep %d \r", lightSleepResult);
}

void backlightControllerTask(void *p)
{
    while (true)
    {
        backlightController((BackligthControllerParameters *)p);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

void touchScreenMonitorTask(void *p)
{
    while (true)
    {
        touchScreenMonitor((TouchScreenMonitorParameters *)p);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

void noEventsMonitorTask(void *p)
{
    while (true)
    {
        noEventsMonitor((NoEventsMonitorParameters *)p);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

BackligthControllerParameters backlightControllerParameters;
NoEventsMonitorParameters noEventsMonitorParameters;
TouchScreenMonitorParameters touchScreenMonitorParameters;

SemaphoreHandle_t lastTouchTimestampMutex = NULL;
time_t lastTouchTimestamp = 0;

SemaphoreHandle_t backlightLevelMutex = NULL;
uint8_t backlightLevel = 8;

// void IRAM_ATTR isr() {
//     Serial.println("AXP202 interrupt");
//     watch->power->readIRQ();
//     watch->power->clearIRQ();
// }

SemaphoreHandle_t touchScreenMonitorTaskMutex = NULL;
TaskParameters touchScreenMonitorTaskParameters;

void setup()
{
    //setCpuFrequencyMhz(20);

    Serial.begin(115200);

    // Get Watch object and set up the display
    watch = TTGOClass::getWatch();
    watch->begin();
    watch->openBL();
    watch->setBrightness(8);

    //Check if the RTC clock matches, if not, use compile time
    watch->rtc->check();
    //Synchronize time to system time
    watch->rtc->syncToSystem();

    WiFi.mode(WIFI_OFF);

    lastTouchTimestampMutex = xSemaphoreCreateMutex();
    backlightLevelMutex = xSemaphoreCreateMutex();

    // Turn on the IRQ used
    //watch->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    //watch->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    //watch->power->clearIRQ();

    // Interrupt that allows you to lightly sleep or wake up the screen
    // pinMode(AXP202_INT, INPUT);
    // attachInterrupt(AXP202_INT, isr, FALLING);
    // watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    // watch->power->clearIRQ();

    // Turn off unused power
    watch->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF); // audio device
    watch->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);

    xTaskCreate(showClock, "showClock", 2048, NULL, 1, NULL);

    touchScreenMonitorParameters = {
        .lastTouchTimestampMutex = &lastTouchTimestampMutex,
        .lastTouchTimestamp = &lastTouchTimestamp,
        .getTouched = getTouched,
        .time = time,
        .take = take,
        .give = give,
        .log = log};
    touchScreenMonitorTaskMutex = xSemaphoreCreateMutex();
    touchScreenMonitorTaskParameters = create(touchScreenMonitor, &touchScreenMonitorParameters, &touchScreenMonitorTaskMutex, 250);
    xTaskCreate(task, "touchScreenMonitorTask", 2048, (void *)&touchScreenMonitorTaskParameters, 1, NULL);

    noEventsMonitorParameters = {
        .lastTouchTimestampMutex = &lastTouchTimestampMutex,
        .lastTouchTimestamp = &lastTouchTimestamp,
        .backlightLevelMutex = &backlightLevelMutex,
        .backlightLevel = &backlightLevel,
        .sleep = sleep,
        .take = take,
        .give = give,
        .time = time,
        .log = log};

    xTaskCreate(noEventsMonitorTask, "noEventsMonitorTask", 2048, (void *)&noEventsMonitorParameters, 1, NULL);

    backlightControllerParameters = {
        .backlightLevelMutex = &backlightLevelMutex,
        .backlightLevel = &backlightLevel,
        .setBrightness = setBrightness,
        .take = take,
        .give = give,
        .log = log};
    xTaskCreate(backlightControllerTask, "backlightControllerTask", 2048, (void *)&backlightControllerParameters, 1, NULL);

    Serial.println("tasks started");
    //lightSleep();
}

void loop()
{
    Serial.println("loop");
    vTaskSuspend(NULL);
    // empty
    //exit(0);
    // uint32_t freq = getCpuFrequencyMhz();
    // float current = watch->power->getBattDischargeCurrent();
    // Serial.printf("freq %d current %.2f \r", freq, current);
}