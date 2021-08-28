#include <stdio.h>
#include "component.hpp"
#include "dateComponent.hpp"

void dateComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d", watchState.date.day, watchState.date.month, watchState.date.year);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextSize(2);
    tftApi->setTextFont(1);
    tftApi->setTextColor(0xFDA0, 0x0000);
    tftApi->print(buf);
}

bool dateComponentNewState(Component component, WatchState watchState)
{
    DateComponentState *state = (DateComponentState *)component.state;   

    bool changed = false;

    if (watchState.date.day != state->_day) 
    {
        changed = true;
        state->_day = watchState.date.day;
    }

    if (watchState.date.month != state->_month) 
    {
        changed = true;
        state->_month = watchState.date.month;
    }

    if (watchState.date.year != state->_year) 
    {
        changed = true;
        state->_year = watchState.date.year;
    }

    return changed;
}

Component createDateComponent(signed short x, signed short y, signed short w, signed short h, DateComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .onTouch = componentNoopHandler,
        .onMove = componentNoopHandler,
        .onRelease = componentNoopHandler,
        .render = dateComponentRender,
        .newState = dateComponentNewState,
        .state = state,
    };
}
