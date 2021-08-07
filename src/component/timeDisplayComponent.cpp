#include <stdio.h>
#include "component.hpp"
#include "timeDisplayComponent.hpp"

void timeDisplayComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    TimeDisplayComponentState *state = (TimeDisplayComponentState *)component.state;    
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
    tftApi->fillRect(component.x, component.y, component.w, component.h, 0x0000);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextSize(1);
    tftApi->setTextFont(7);
    tftApi->setTextColor(state->color);
    tftApi->print(buf);
}

void timeDisplayComponentOnTouch(Component component)
{
    TimeDisplayComponentState *state = (TimeDisplayComponentState *)component.state;   
    if (state->color == 0xFFFF) {
        state->color = 0x9999;
    } else {
        state->color = 0xFFFF;
    }
}

bool timeDisplayComponentNewState(Component component, WatchState watchState)
{
    TimeDisplayComponentState *state = (TimeDisplayComponentState *)component.state;   

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

    if (watchState.date.minute != state->_minute) 
    {
        changed = true;
        state->_minute = watchState.date.minute;
    }

    if (watchState.date.hour != state->_hour) 
    {
        changed = true;
        state->_hour = watchState.date.hour;
    }

    return changed;
}

Component createTimeDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, TimeDisplayComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .state = state,
        .render = timeDisplayComponentRender,
        .onTouch = timeDisplayComponentOnTouch,
        .newState = componentNewState,
    };
}