#pragma once

#include "component.hpp"

typedef struct
{
    int _battPercentage;
} BatteryDisplayComponentState;

Component createBatteryDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, BatteryDisplayComponentState *state);