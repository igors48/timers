#pragma once

#include "component.hpp"

typedef struct
{
    int _battPercentage;
} BatteryDisplayComponentState;

Component createBatteryDisplayComponent(signed short x, signed short y, signed short w, signed short h, BatteryDisplayComponentState *state);