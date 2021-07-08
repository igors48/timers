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
TaskHandle_t buttonListenerHandle = NULL;
SemaphoreHandle_t lastShortPressTimestampMutex = NULL;
time_t lastShortPressTimestamp = 0;

void showClock(void *p)
{
    auto tnow = watch->rtc->formatDateTime(PCF_TIMEFORMAT_HMS);
    watch->tft->setCursor(8, 8);
    watch->tft->setTextSize(3);
    watch->tft->setTextFont(2);
    watch->tft->print(tnow);
}

void buttonListener(void *p)
{
    while (true)
    {
        vTaskSuspend(NULL);
        watch->power->readIRQ();
        if (watch->power->isPEKShortPressIRQ())
        {

            Serial.println("PowerKey Press");
        }
        watch->power->clearIRQ();
    }
}

void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();
    watch->begin();
    watch->openBL();
    watch->setBrightness(8);

    watch->rtc->check();
    watch->rtc->syncToSystem();

    WiFi.mode(WIFI_OFF);

    xTaskCreate(buttonListener, "buttonListener", 2048, NULL, 1, &buttonListenerHandle);

    watch->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, buttonInterruptHandler, FALLING);

    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    watch->power->clearIRQ();

    Serial.println("tasks started");
}

void loop()
{
    Serial.println("loop");
    vTaskSuspend(NULL);
}