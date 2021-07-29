#pragma once

#include "../watch/rtc.hpp"

// no pointers, values only
typedef struct 
{
    Date date;
    int battPercentage; // todo consider battery state struct
} WatchState;

WatchState initialWatchState();