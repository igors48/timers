#pragma once

#include "tools/func.hpp"

typedef bool (*GetTouch)(signed short &x, signed short &y);

typedef struct {
    VoidFunc init;
    VoidFunc afterWakeUp;
    VoidFunc beforeGoToSleep;
    VoidFunc goToSleep;
    GetTouch getTouch;
} WatchApi;

WatchApi defaultWatchApi();