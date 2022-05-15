#include <stddef.h>

#include <LilyGoWatch.h>

#include "component.hpp"

void componentNoopHandler(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    // empty
}

void componentGestureNoopHandler(Component *component, Gesture gesture)
{
    // empty
}

bool componentNewState(Component *component)
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
    Serial.println("before component mount");
    component->x += x;
    component->y += y;
    Serial.println("after component mount");
}