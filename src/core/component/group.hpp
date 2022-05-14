#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char childrenCount;
    void **children;  
} GroupState;

void groupRender(Component *group, bool forced, TftApi *tftApi);
bool groupNewState(Component *component);
Component* groupContains(Component *component, signed short x, signed short y);
void groupMount(Component *component, signed short x, signed short y);
