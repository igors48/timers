#pragma once

#include "watch/watch.hpp"

typedef struct
{
    TTGOClass *watch;
} ShowClockParameters;

void showClock(ShowClockParameters *p);