#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char childrenCount;
    void **children; 
    Render forcedRender;   
} GroupState;

GroupState createGroupState(unsigned char childrenCount, void **children);

Component createGroupComponent(signed short x, signed short y, GroupState *state);