#include <stdio.h>

#include "component.hpp"
#include "touchDisplayComponent.hpp"

void touchDisplayComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "x:%03d, y:%03d", watchState.touchX, watchState.touchY);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextFont(1);
    tftApi->setTextSize(1);
    tftApi->setTextColor(0xFDA0, 0x0000);
    tftApi->print(buf);
}

bool touchDisplayComponentNewState(Component component, WatchState watchState)
{
    TouchDisplayComponentState *state = (TouchDisplayComponentState *)component.state;   
    bool changed = false;
    if (state->_touchX != watchState.touchX)
    {
        changed = true;
        state->_touchX = watchState.touchX;
    }
    if (state->_touchY != watchState.touchY)
    {
        changed = true;
        state->_touchY = watchState.touchY;
    }
    return changed;
}

Component createTouchDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, TouchDisplayComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .onTouch = componentNoopHandler,
        .onMove = componentNoopHandler,
        .onRelease = componentNoopHandler,
        .render = touchDisplayComponentRender,
        .newState = touchDisplayComponentNewState,
        .state = state,
    };
}