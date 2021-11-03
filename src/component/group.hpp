#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char childrenCount;
    void **children;    
} GroupState;

void groupRender(Component *group, WatchState *watchState, TftApi *tftApi);

bool groupNewState(Component *component, WatchState *watchState);

Component* groupContains(Component *component, signed short x, signed short y);

Component createGroupComponent(signed short x, signed short y, signed short w, signed short h, GroupState *state);