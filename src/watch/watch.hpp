#pragma once

#include "tools/func.hpp"

typedef bool (*GetTouch)(signed short &x, signed short &y);
typedef void (*GoToSleep)(unsigned long sleepTimeMicros);

typedef struct {
    VoidFunc init;
    VoidFunc afterWakeUp;
    VoidFunc beforeGoToSleep;
    GoToSleep goToSleep;
    GetTouch getTouch;
} WatchApi;

WatchApi defaultWatchApi();