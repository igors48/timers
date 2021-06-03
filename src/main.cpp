#include <Arduino.h>
// Select you T-Watch in the platformio.ini file
#include <LilyGoWatch.h>
#include <WiFi.h>

#include "backlightController.hpp"
#include "noEventsMonitor.hpp"
#include "touchScreenMonitor.hpp"
#include "freertos.hpp"

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
        touchScreenMonitor((TouchScreenMonitorParameters *) p);
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

void setup()
{
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

    xTaskCreate(touchScreenMonitorTask, "touchScreenMonitorTask", 2048, (void *)&touchScreenMonitorParameters, 1, NULL);

    noEventsMonitorParameters = {
        .lastTouchTimestampMutex = &lastTouchTimestampMutex,
        .lastTouchTimestamp = &lastTouchTimestamp,
        .backlightLevelMutex = &backlightLevelMutex,
        .backlightLevel = &backlightLevel,
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
}

void loop()
{
    // empty
}