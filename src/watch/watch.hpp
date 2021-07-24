#pragma once

#include <LilyGoWatch.h>

#include "tools/func.hpp"

extern TTGOClass *watch;

// todo extract to different file without arduino and lilygo deps
typedef struct {
    VoidFunc init;
    VoidFunc afterWakeUp;
    VoidFunc beforeGoToSleep;
    VoidFunc goToSleep;
} WatchApi;

WatchApi defaultWatchApi();