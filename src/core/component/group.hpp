#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char childrenCount;
    void **children;  
} GroupState;

void groupRender(Component *group, bool forced, TftApi *tftApi);
Component* groupContains(Component *component, signed short x, signed short y);
void groupMount(Component *component, signed short x, signed short y);
bool groupIsStateModified(Component *component);
void groupUpdateState(Component *component);
