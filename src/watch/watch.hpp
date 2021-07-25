#pragma once

#include "tools/func.hpp"

typedef struct {
    VoidFunc init;
    VoidFunc afterWakeUp;
    VoidFunc beforeGoToSleep;
    VoidFunc goToSleep;
} WatchApi;

WatchApi defaultWatchApi();