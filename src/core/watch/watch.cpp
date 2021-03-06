/**
 * @file
 * @author Igor Usenko (github: igors48)
 * @brief default implementation of watch device related functions
 */
#include <WiFi.h>
#include <Arduino.h>
#include "driver/i2s.h"

#include "ttgo.hpp"
#include "watch.hpp"

/**
 * @brief init accelerometer chip
 */
static void initBma()
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

/**
 * @brief init sound I2S interface
 */
static void initI2S()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 8000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S | I2S_COMM_FORMAT_STAND_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 1024,
        .use_apll = false,
    };
    esp_err_t error = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    Serial.printf("after i2s driver install %d \r\n", error);

    i2s_pin_config_t i2s_cfg;
    i2s_cfg.bck_io_num = TWATCH_DAC_IIS_BCK;
    i2s_cfg.ws_io_num = TWATCH_DAC_IIS_WS;
    i2s_cfg.data_out_num = TWATCH_DAC_IIS_DOUT;
    i2s_cfg.data_in_num = I2S_PIN_NO_CHANGE;
    delay(50);
    error = i2s_set_pin(I2S_NUM_0, &i2s_cfg);
    Serial.printf("after i2s set pin %d \r\n", error);
}

/**
 * @copydoc WatchApi.init
 */
static void initWatch()
{
    watch = TTGOClass::getWatch();
    watch->begin();

    RTC_Date startYear = RTC_Date(2022, 1, 1, 0, 0, 0);
    watch->rtc->setDateTime(startYear);

    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    watch->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    watch->power->clearIRQ();

    WiFi.mode(WIFI_OFF);

    initBma();

    watch->motor_begin();

    initI2S();
}

/**
 * @copydoc WatchApi.afterWakeUp
 */
static void afterWakeUp()
{
    watch->bma->readInterrupt();
    watch->openBL();
    watch->displayWakeup();
    watch->setBrightness(255);
    watch->touchWakup();

    watch->power->setPowerOutPut(AXP202_LDO4, true); // audio power
}

/**
 * @copydoc WatchApi.beforeGoToSleep
 */
static void beforeGoToSleep()
{
    watch->closeBL();
    watch->displaySleep();
    watch->touchToSleep();
    watch->powerOff();
}

/**
 * @copydoc WatchApi.goToSleep
 */
static WakeUpReason goToSleep(unsigned long sleepTimeMicros)
{
    esp_sleep_enable_ext0_wakeup((gpio_num_t)AXP202_INT, LOW);
    delay(100); // seen some false wake ups without those delays
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    delay(100);
    esp_sleep_enable_timer_wakeup(sleepTimeMicros); // todo subtract those delays from sleep time #129
    delay(100);
    watch->bma->readInterrupt();
    delay(100);
    watch->power->clearIRQ();
    delay(100);
    // esp_deep_sleep_start();
    esp_light_sleep_start();
    esp_sleep_wakeup_cause_t wakeupReason = esp_sleep_get_wakeup_cause();
    Serial.printf("after esp_light_sleep_start. wakeup reason %d\r\n", wakeupReason);
    switch (wakeupReason)
    {
    case ESP_SLEEP_WAKEUP_UNDEFINED:
        return WUR_SLEEP_WAKEUP_UNDEFINED;
    case ESP_SLEEP_WAKEUP_ALL:
        return WUR_SLEEP_WAKEUP_ALL;
    case ESP_SLEEP_WAKEUP_EXT0:
        return WUR_SLEEP_WAKEUP_EXT0;
    case ESP_SLEEP_WAKEUP_EXT1:
        return WUR_SLEEP_WAKEUP_EXT1;
    case ESP_SLEEP_WAKEUP_TIMER:
        return WUR_SLEEP_WAKEUP_TIMER;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        return WUR_SLEEP_WAKEUP_TOUCHPAD;
    case ESP_SLEEP_WAKEUP_ULP:
        return WUR_SLEEP_WAKEUP_ULP;
    case ESP_SLEEP_WAKEUP_GPIO:
        return WUR_SLEEP_WAKEUP_GPIO;
    case ESP_SLEEP_WAKEUP_UART:
        return WUR_SLEEP_WAKEUP_UART;
    case ESP_SLEEP_WAKEUP_WIFI:
        return WUR_SLEEP_WAKEUP_WIFI;
    case ESP_SLEEP_WAKEUP_COCPU:
        return WUR_SLEEP_WAKEUP_COCPU;
    case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG:
        return WUR_SLEEP_WAKEUP_COCPU_TRAP_TRIG;
    case ESP_SLEEP_WAKEUP_BT:
        return WUR_SLEEP_WAKEUP_BT;
    default:
        return WUR_UNKNOWN;
    }
}

/**
 * @copydoc WatchApi.getTouch
 */
static bool getTouch(signed short &x, signed short &y)
{
    return watch->getTouch(x, y);
}

/**
 * @copydoc WatchApi.setBrightness
 */
static void setBrightness(unsigned char level)
{
    watch->setBrightness(level);
}

WatchApi defaultWatchApi()
{
    return {
        .init = initWatch,
        .afterWakeUp = afterWakeUp,
        .beforeGoToSleep = beforeGoToSleep,
        .goToSleep = goToSleep,
        .getTouch = getTouch,
        .setBrightness = setBrightness,
    };
}