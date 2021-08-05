#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char x;
    unsigned char y;
} BatteryDisplayComponentState;

Component createBatteryDisplayComponent(BatteryDisplayComponentState *state);