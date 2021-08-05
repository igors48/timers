#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char x;
    unsigned char y;
    unsigned char w;
    unsigned char h;
} BatteryDisplayComponentState;

Component createBatteryDisplayComponent(BatteryDisplayComponentState *state);