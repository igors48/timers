#include <stddef.h>

#include "component.hpp"

void componentNoopHandler(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    // empty
}

void componentGestureNoopHandler(Component *component, Gesture gesture)
{
    // empty
}

void componentButtonNoopHandler(Component *component)
{
    // empty
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