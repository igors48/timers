#include <WiFi.h>
#include <Arduino.h>

#include "ttgo.hpp"
#include "watch.hpp"

void watchInitBma()
{
    BMA *sensor = watch->bma;
    Acfg cfg;
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    cfg.range = BMA4_ACCEL_RANGE_2G;
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;
    sensor->accelConfig(cfg);
    sensor->enableAccel();
    sensor->enableFeature(BMA423_STEP_CNTR, true);
    sensor->enableFeature(BMA423_TILT, true);
    sensor->enableFeature(BMA423_WAKEUP, true);
    sensor->resetStepCounter();
    sensor->enableTiltInterrupt();
    sensor->enableWakeupInterrupt();
}

void watchInit()
{
    watch = TTGOClass::getWatch();
    watch->begin();

    //Serial.println(__DATE__);
    Serial.println(__DATE__);
    Serial.println(__TIME__);
    RTC_Date compiled = RTC_Date(__DATE__, __TIME__); // seems __DATE__, __TIME__ set to compilation time for this file not the project
    watch->rtc->setDateTime(compiled);

    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    watch->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    watch->power->clearIRQ();

    WiFi.mode(WIFI_OFF);

    watchInitBma();
}

void watchAfterWakeUp()
{
    watch->bma->readInterrupt();
    watch->openBL();
    watch->displayWakeup();
    watch->setBrightness(128);
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
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    delay(100);
    watch->bma->readInterrupt();
    delay(100);
    watch->power->clearIRQ();
    delay(100); // seen some false wake ups without it
    //esp_deep_sleep_start();
    esp_light_sleep_start();
    Serial.println("after esp_light_sleep_start");
}

bool watchGetTouch(signed short &x, signed short &y){
    return watch->getTouch(x, y);
}

WatchApi defaultWatchApi()
{
    return
    {
        .init = watchInit,
        .afterWakeUp = watchAfterWakeUp,
        .beforeGoToSleep = watchBeforeGoToSleep,
        .goToSleep = watchGoToSleep,
        .getTouch = watchGetTouch
    };
}