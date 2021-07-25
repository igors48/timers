#pragma once

#include "../watch/rtc.hpp"

// no pointers, values only
typedef struct 
{
    Date date;
} WatchState;

WatchState copy(WatchState watchState);