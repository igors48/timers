#pragma once

#include "watch/ttgo.hpp"

typedef struct
{
    TTGOClass *watch;
} ShowClockParameters;

void showClock(void *p);