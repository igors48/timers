#pragma once

#include "../watch/rtc.hpp"

// no pointers, values only
typedef struct 
{
    Date date;
    int battPercentage; // todo consider battery state struct
    signed short touchX;
    signed short touchY;
    unsigned int stepCount;
} WatchState;

WatchState initialWatchState();