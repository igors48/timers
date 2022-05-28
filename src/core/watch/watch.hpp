#pragma once

#include "core/tools/func.hpp"

typedef enum {
    WUR_SLEEP_WAKEUP_UNDEFINED,    //!< In case of deep sleep, reset was not caused by exit from deep sleep
    WUR_SLEEP_WAKEUP_ALL,          //!< Not a wakeup cause, used to disable all wakeup sources with esp_sleep_disable_wakeup_source
    WUR_SLEEP_WAKEUP_EXT0,         //!< Wakeup caused by external signal using RTC_IO
    WUR_SLEEP_WAKEUP_EXT1,         //!< Wakeup caused by external signal using RTC_CNTL
    WUR_SLEEP_WAKEUP_TIMER,        //!< Wakeup caused by timer
    WUR_SLEEP_WAKEUP_TOUCHPAD,     //!< Wakeup caused by touchpad
    WUR_SLEEP_WAKEUP_ULP,          //!< Wakeup caused by ULP program
    WUR_SLEEP_WAKEUP_GPIO,         //!< Wakeup caused by GPIO (light sleep only)
    WUR_SLEEP_WAKEUP_UART,         //!< Wakeup caused by UART (light sleep only)
    WUR_SLEEP_WAKEUP_WIFI,              //!< Wakeup caused by WIFI (light sleep only)
    WUR_SLEEP_WAKEUP_COCPU,             //!< Wakeup caused by COCPU int
    WUR_SLEEP_WAKEUP_COCPU_TRAP_TRIG,   //!< Wakeup caused by COCPU crash
    WUR_SLEEP_WAKEUP_BT,           //!< Wakeup caused by BT (light sleep only)
    WUR_UNKNOWN,
} WakeUpReason;

typedef struct {
    void (*init)();
    void (*afterWakeUp)();
    void (*beforeGoToSleep)();
    WakeUpReason (*goToSleep)(unsigned long sleepTimeMicros);
    bool (*getTouch)(signed short &x, signed short &y);
} WatchApi;

WatchApi defaultWatchApi();