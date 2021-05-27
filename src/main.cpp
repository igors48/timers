#include <Arduino.h>
// Select you T-Watch in the platformio.ini file
#include <LilyGoWatch.h>
#include <WiFi.h>

// C++ object which will allow access to the functions of the Watch
TTGOClass *watch;

void showBattState(void *pvParameters)
{
    uint8_t per;
    float vbus_v, vbus_c, batt_v;
    while (true)
    {
        vbus_v = watch->power->getVbusVoltage();
        vbus_c = watch->power->getVbusCurrent();
        batt_v = watch->power->getBattVoltage();
        per = watch->power->getBattPercentage();
        watch->tft->setCursor(0, 180);
        watch->tft->setTextSize(1);
        watch->tft->setTextFont(1);
        //watch->tft->setTextColor(TFT_GREEN);
        if (watch->power->isVBUSPlug())
        {
            watch->tft->print("P ");
        }
        else
        {
            watch->tft->print("U ");
        }
        watch->tft->setCursor(0, 190);
        if (watch->power->isChargeing())
        {
            watch->tft->print("C: ");
            watch->tft->print(watch->power->getBattChargeCurrent());
        }
        else
        {
            // Show current consumption
            watch->tft->print("D: ");
            watch->tft->print(watch->power->getBattDischargeCurrent());
        }
        watch->tft->print(" mA ");

        // Print the values
        watch->tft->setCursor(0, 200);
        watch->tft->print("BV: ");
        watch->tft->print(vbus_v);
        watch->tft->print(" mV ");
        watch->tft->setCursor(0, 210);
        watch->tft->print("BC: ");
        watch->tft->print(vbus_c);
        watch->tft->print(" mA ");
        watch->tft->setCursor(0, 220);
        watch->tft->print("Vbat: ");
        watch->tft->print(batt_v);
        watch->tft->print(" mV ");
        watch->tft->setCursor(0, 230);
        //watch->tft->print("Percent: ");
        watch->tft->print(per);
        watch->tft->print(" %");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

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

void showTouch(void *pvParameters)
{
    int16_t x, y;
    x = 0;
    y = 0;
    while (true)
    {
        if (watch->getTouch(x, y))
        {
            watch->tft->fillCircle(x, y, 5, TFT_WHITE);
        }

        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

SemaphoreHandle_t lastTouchTimestampMutex = NULL;
time_t lastTouchTimestamp = 0;

void touchScreenMonitor(void *pvParameters)
{
    while (true)
    {
        if (lastTouchTimestampMutex == NULL)
        {
            Serial.println("lastTouchTimestampMutex = NULL");
        }
        else
        {
            uint8_t touched = watch->touch->getTouched();
            if (touched)
            {
                time_t timestamp = time(NULL);
                if (xSemaphoreTake(lastTouchTimestampMutex, (TickType_t)10) == pdTRUE)
                {
                    lastTouchTimestamp = timestamp;
                    xSemaphoreGive(lastTouchTimestampMutex);
                    Serial.printf("lastTouchTimestamp set to %lu \r\n", lastTouchTimestamp);
                }
                else
                {
                    Serial.println("lastTouchTimestampMutex couldnt obtain from touchScreenMonitor");
                }
            }
        }
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

SemaphoreHandle_t backlightLevelMutex = NULL;
uint8_t backlightLevel = 8;

typedef struct
{
    SemaphoreHandle_t *lastTouchTimestampMutex;
    time_t *lastTouchTimestamp;
    SemaphoreHandle_t *backlightLevelMutex;
    uint8_t *backlightLevel;
} NoEventsMonitorParameters;

void noEventsMonitor(NoEventsMonitorParameters *p)
{
    if (xSemaphoreTake(*p->lastTouchTimestampMutex, (TickType_t)10) == pdTRUE)
    {
        time_t last = *p->lastTouchTimestamp;
        xSemaphoreGive(*p->lastTouchTimestampMutex);
        time_t current = time(NULL);
        time_t diff = current - last;
        uint8_t level = 0;
        if (diff < 5)
        {
            level = 128;
        }
        else
        {
            level = 8;
        }
        if (xSemaphoreTake(*p->backlightLevelMutex, (TickType_t)10) == pdTRUE)
        {
            *p->backlightLevel = level;
            xSemaphoreGive(*p->backlightLevelMutex);
            Serial.printf("backlightLevel set to %d \r\n", level);
        }
        else
        {
            Serial.println("backlightLevelMutex couldnt obtain from noEventsMonitor");
        }
    }
    else
    {
        Serial.println("lastTouchTimestampMutex couldnt obtain from noEventsMonitor");
    }
}

void noEventsMonitorTask(void *pvParameters)
{
    while (true)
    {
        noEventsMonitor((NoEventsMonitorParameters *)pvParameters);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

typedef struct
{
    SemaphoreHandle_t *backlightLevelMutex;
    uint8_t *backlightLevel;
    TTGOClass *watch;
} BackligthControllerParameters;

void backlightController(BackligthControllerParameters *p)
{
    if (xSemaphoreTake(*p->backlightLevelMutex, (TickType_t)10) == pdTRUE)
    {
        uint8_t current = *p->backlightLevel;
        xSemaphoreGive(*p->backlightLevelMutex);
        //todo - mutex needed here?
        p->watch->setBrightness(current);
        Serial.printf("brightness set to %d \r\n", current);
    }
    else
    {
        Serial.println("backlightLevelMutex couldnt obtain from noEventsMonitor");
    }
}

void backlightControllerTask(void *p)
{
    while (true)
    {
        backlightController((BackligthControllerParameters *)p);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

BackligthControllerParameters backlightControllerParameters;
NoEventsMonitorParameters noEventsMonitorParameters;

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

    //xTaskCreate(showBattState, "showBattState", 2048, NULL, 1, NULL);
    xTaskCreate(showClock, "showClock", 2048, NULL, 1, NULL);
    //xTaskCreate(showTouch, "showTouch", 2048, NULL, 1, NULL);
    xTaskCreate(touchScreenMonitor, "touchScreenMonitor", 2048, NULL, 1, NULL);

    noEventsMonitorParameters = {
        .lastTouchTimestampMutex = &lastTouchTimestampMutex,
        .lastTouchTimestamp = &lastTouchTimestamp,
        .backlightLevelMutex = &backlightLevelMutex,
        .backlightLevel = &backlightLevel};
    xTaskCreate(noEventsMonitorTask, "noEventsMonitorTask", 2048, (void *)&noEventsMonitorParameters, 1, NULL);

    backlightControllerParameters = {
        .backlightLevelMutex = &backlightLevelMutex,
        .backlightLevel = &backlightLevel,
        .watch = watch};

    xTaskCreate(backlightControllerTask, "backlightControllerTask", 2048, (void *)&backlightControllerParameters, 1, NULL);
    Serial.println("tasks started");
}

void loop()
{
    // showBattState();
    // showClock();
    // delay(250);
}