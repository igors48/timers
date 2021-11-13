#include <stddef.h>

#include "component.hpp"

void componentNoopHandler(Component *component, signed short x, signed short y)
{
    // empty
}

bool componentNewState(Component *component, WatchState *watchState)
{
    return true;
}

Component* componentContains(Component *component, signed short x, signed short y)
{
    if ((x >= component->x) && (x <= (component->x + component->w)) && (y >= component->y) && (y <= (component->y + component->h)))
    {
        return component;
    }
    return NULL;
}

void componentMount(Component *component, signed short x, signed short y)
{
    component->x += x;
    component->y += y;
}