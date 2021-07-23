#pragma once

#include <LilyGoWatch.h>

#include "tools/func.hpp"

extern TTGOClass *watch;

typedef struct {
    VoidFunc init;
    VoidFunc deepSleep;
} WatchApi;

WatchApi defaultWatchApi();