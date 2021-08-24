#include <stdio.h>
#include "component.hpp"
#include "secondComponent.hpp"

void secondComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    SecondComponentState *state = (SecondComponentState *)component.state;
    char buf[16];
    snprintf(buf, sizeof(buf), ":%02d", watchState.date.second);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextSize(1);
    tftApi->setTextFont(7);
    tftApi->setTextColor(state->color, 0x0000);
    tftApi->print(buf);
}

void secondComponentOnTouch(Component component)
{
    SecondComponentState *state = (SecondComponentState *)component.state;
    if (state->color == 0xFDA0)
    {
        state->color = 0x9999;
    }
    else
    {
        state->color = 0xFDA0;
    }
}

bool secondComponentNewState(Component component, WatchState watchState)
{
    SecondComponentState *state = (SecondComponentState *)component.state;

    bool changed = false;

    if (state->color != state->_color)
    {
        changed = true;
        state->_color = state->color;
    }

    if (watchState.date.second != state->_second)
    {
        changed = true;
        state->_second = watchState.date.second;
    }

    return changed;
}

Component createSecondComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, SecondComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .state = state,
        .render = secondComponentRender,
        .onTouch = secondComponentOnTouch,
        .newState = secondComponentNewState,
    };
}

void secondComponentColorChange(SecondComponentState *state)
{
    if (state->color == 0xFDA0)
    {
        state->color = 0x9999;
    }
    else
    {
        state->color = 0xFDA0;
    }
}