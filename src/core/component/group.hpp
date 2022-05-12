#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char childrenCount;
    void **children;  
} GroupState;

GroupState createGroupState(unsigned char childrenCount, void **children);

GroupState* createGroupStateRef(unsigned char childrenCount, void **children);

Component createGroupComponent(signed short x, signed short y, GroupState *state);

Component* createGroupComponentRef(signed short x, signed short y, GroupState *state);

