#include <stdio.h>
#include "component.hpp"
#include "secondComponent.hpp"

void secondComponentRender(Component *component, WatchState *watchState, TftApi *tftApi)
{
    SecondComponentState *state = (SecondComponentState *)component->state;
    char buf[16];
    snprintf(buf, sizeof(buf), ":%02d", watchState->date.second);
    tftApi->setCursor(component->x, component->y);
    tftApi->setTextSize(1);
    tftApi->setTextFont(7);
    tftApi->setTextColor(state->color, COLOR_BLACK);
    tftApi->print(buf);
}

void secondComponentOnRelease(Component *component, signed short x, signed short y)
{
    SecondComponentState *state = (SecondComponentState *)(component->state);
    if (state->color == COLOR_INFORMATION)
    {
        state->color = COLOR_INTERACTION;
    }
    else
    {
        state->color = COLOR_INFORMATION;
    }
}

bool secondComponentNewState(Component *component, WatchState *watchState)
{
    SecondComponentState *state = (SecondComponentState *)component->state;

    bool changed = false;

    if (state->color != state->_color)
    {
        changed = true;
        state->_color = state->color;
    }

    if (watchState->date.second != state->_second)
    {
        changed = true;
        state->_second = watchState->date.second;
    }

    return changed;
}

Component createSecondComponent(signed short x, signed short y, signed short w, signed short h, SecondComponentState *state)
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
        .onRelease = secondComponentOnRelease,
        .render = secondComponentRender,
        .newState = secondComponentNewState,
        .state = state,
    };
}

void secondComponentColorChange(SecondComponentState *state)
{
    if (state->color == COLOR_INFORMATION)
    {
        state->color = COLOR_INTERACTION;
    }
    else
    {
        state->color = COLOR_INFORMATION;
    }
}