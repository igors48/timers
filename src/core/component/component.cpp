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

void componentRenderNoop(Component *component, bool forced, TftApi *tftApi)
{
    // empty
}

bool componentIsStateModifiedNoop(Component *component)
{
    return false;
}

void componentUpdateStateNoop(Component *component)
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

Component createComponent(signed short x, signed short y, signed short w, signed short h, void *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .contains = componentContains,
        .mount = componentMount,
        .onTouch = componentNoopHandler,
        .onMove = componentNoopHandler,
        .onRelease = componentNoopHandler,
        .onGesture = componentGestureNoopHandler,
        .onButton = componentButtonNoopHandler,
        .render = componentRenderNoop,
        .isStateModified = componentIsStateModifiedNoop,
        .updateState = componentUpdateStateNoop,
        .state = state,
    };  
}

