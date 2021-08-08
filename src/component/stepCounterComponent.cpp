#include <stdio.h>

#include "component.hpp"
#include "stepCounterComponent.hpp"

void stepCounterComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "S:%05d", watchState.stepCount);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextFont(1);
    tftApi->setTextSize(2);
    tftApi->setTextColor(0xFFFF, 0x0000);
    tftApi->print(buf);
}

bool stepCounterComponentNewState(Component component, WatchState watchState)
{
    StepCounterComponentState *state = (StepCounterComponentState *)component.state;   
    bool changed = false;
    if (state->_stepCount != watchState.stepCount) 
    {
        changed = true;
        state->_stepCount = watchState.stepCount;
    }
    return changed;
}

Component createStepCounterComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, StepCounterComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .state = state,
        .render = stepCounterComponentRender,
        .onTouch = componentOnTouch,
        .newState = stepCounterComponentNewState,
    };
}