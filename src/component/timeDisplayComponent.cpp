#include <stdio.h>

#include "component.hpp"
#include "timeDisplayComponent.hpp"

void timeDisplayComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    TimeDisplayComponentState *state = (TimeDisplayComponentState *)component.state;    
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
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
        state->color = 0x0000;
    } else {
        state->color = 0xFFFF;
    }
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
    };
}