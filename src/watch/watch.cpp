#include <WiFi.h>
#include <Arduino.h>

#include "ttgo.hpp"
#include "watch.hpp"

void watchInit()
{
    watch = TTGOClass::getWatch();
    watch->begin();

    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    watch->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    watch->power->clearIRQ();

    watch->rtc->check();
    watch->rtc->syncToSystem();

    WiFi.mode(WIFI_OFF);

    // watch->tft->setCursor(8, 8);
    // watch->tft->setTextSize(3);
    // watch->tft->setTextFont(2);
    // watch->tft->print("ON");
}

void watchAfterWakeUp()
{
    watch->openBL();
    watch->displayWakeup();
    watch->setBrightness(8);
    watch->touchWakup();

    watch->rtc->check();
    watch->rtc->syncToSystem();
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