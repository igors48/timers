#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char childrenCount;
    void **children;    
} GroupState;

Component createBatteryDisplayComponent(signed short x, signed short y, signed short w, signed short h, GroupState *state);