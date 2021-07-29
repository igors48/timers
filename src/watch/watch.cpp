#include <WiFi.h>
#include <Arduino.h>

#include "ttgo.hpp"
#include "watch.hpp"

void watchInit()
{
    watch = TTGOClass::getWatch();
    watch->begin();

    RTC_Date compiled = RTC_Date(__DATE__, __TIME__);
    watch->rtc->setDateTime(compiled);

    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    watch->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    watch->power->clearIRQ();

    WiFi.mode(WIFI_OFF);
}

void watchAfterWakeUp()
{
    watch->openBL();
    watch->displayWakeup();
    watch->setBrightness(8);
    watch->touchWakup();
}

void watchBeforeGoToSleep()
{
    watch->closeBL();
    watch->displaySleep();
    watch->touchToSleep();
    watch->powerOff();
}

void watchGoToSleep()
{
    esp_sleep_enable_ext0_wakeup((gpio_num_t)AXP202_INT, LOW);
    delay(100);
    watch->power->clearIRQ();
    delay(100); // seen some false wake ups without it
    //esp_deep_sleep_start();
    esp_light_sleep_start();
    Serial.println("after esp_light_sleep_start");
}

WatchApi defaultWatchApi()
{
    return
    {
        .init = watchInit,
        .afterWakeUp = watchAfterWakeUp,
        .beforeGoToSleep = watchBeforeGoToSleep,
        .goToSleep = watchGoToSleep
    };
}