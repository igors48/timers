#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char childrenCount;
    void **children;    
} GroupState;

Component createGroupComponent(signed short x, signed short y, GroupState *state);