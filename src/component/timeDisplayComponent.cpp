#include <stdio.h>

#include "component.hpp"
#include "timeDisplayComponent.hpp"

void timeDisplayComponentRender(void *componentsState, WatchState watchState, TftApi *tftApi)
{
    TimeDisplayComponentState *state = (TimeDisplayComponentState *)componentsState;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
    tftApi->setCursor(state->x, state->y);
    tftApi->setTextSize(1);
    tftApi->setTextFont(7);
    tftApi->print(buf);
}

Component createTimeDisplayComponent(TimeDisplayComponentState *state)
{
    return {
        .state = state,
        .render = timeDisplayComponentRender,
    };
}