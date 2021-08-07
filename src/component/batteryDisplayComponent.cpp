#include <stdio.h>

#include "component.hpp"
#include "batteryDisplayComponent.hpp"

void batteryDisplayComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d %%", watchState.battPercentage);
    tftApi->fillRect(component.x, component.y, component.w, component.h, 0x0000);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextFont(1);
    tftApi->setTextSize(2);
    tftApi->setTextColor(0xFFFF);
    tftApi->print(buf);
}

bool batteryDisplayComponentNewState(Component component, WatchState watchState)
{
    BatteryDisplayComponentState *state = (BatteryDisplayComponentState *)component.state;   
    bool changed = false;
    if (state->_battPercentage != watchState.battPercentage) 
    {
        changed = true;
        state->_battPercentage = watchState.battPercentage;
    }
    return changed;
}

Component createBatteryDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, BatteryDisplayComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .state = state,
        .render = batteryDisplayComponentRender,
        .onTouch = componentOnTouch,
        .newState = batteryDisplayComponentNewState,
    };
}