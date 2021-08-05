#include <stdio.h>

#include "component.hpp"
#include "batteryDisplayComponent.hpp"

void batteryDisplayComponentRender(void *componentsState, WatchState watchState, TftApi *tftApi)
{
    BatteryDisplayComponentState *state = (BatteryDisplayComponentState *)componentsState;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d %%", watchState.battPercentage);
    tftApi->setCursor(state->x, state->y);
    tftApi->setTextFont(1);
    tftApi->setTextSize(2);
    tftApi->print(buf);
}

Component createBatteryDisplayComponent(BatteryDisplayComponentState *state)
{
    return {
        .state = state,
        .render = batteryDisplayComponentRender,
    };
}