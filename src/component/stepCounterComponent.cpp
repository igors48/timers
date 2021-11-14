#include <stdio.h>

#include "component.hpp"
#include "stepCounterComponent.hpp"

void stepCounterComponentRender(Component *component, WatchState *watchState, TftApi *tftApi)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "S:%05d", watchState->stepCount);
    tftApi->setCursor(component->x, component->y);
    tftApi->setTextFont(1);
    tftApi->setTextSize(2);
    tftApi->setTextColor(COLOR_INFORMATION, COLOR_BLACK);
    tftApi->print(buf);
}

bool stepCounterComponentNewState(Component *component, WatchState *watchState)
{
    StepCounterComponentState *state = (StepCounterComponentState *)component->state;   
    bool changed = false;
    if (state->_stepCount != watchState->stepCount) 
    {
        changed = true;
        state->_stepCount = watchState->stepCount;
    }
    return changed;
}

Component createStepCounterComponent(signed short x, signed short y, signed short w, signed short h, StepCounterComponentState *state)
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
        .render = stepCounterComponentRender,
        .newState = stepCounterComponentNewState,
        .state = state,
    };
}