#include <stdio.h>

#include "component.hpp"
#include "touchDisplayComponent.hpp"

void touchDisplayComponentRender(void *componentsState, WatchState watchState, TftApi *tftApi)
{
    TouchDisplayComponentState *state = (TouchDisplayComponentState *)componentsState;
    char buf[16];

    snprintf(buf, sizeof(buf), "x:%03d, y:%03d", watchState.touchX, watchState.touchY);
    tftApi->setCursor(state->x, state->y);
    tftApi->setTextFont(1);
    tftApi->setTextSize(2);
    tftApi->print(buf);
}

Component createTouchDisplayComponent(TouchDisplayComponentState *state)
{
    return {
        .state = state,
        .render = touchDisplayComponentRender,
    };
}