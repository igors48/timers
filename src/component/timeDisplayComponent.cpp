#include <stdio.h>

#include "component.hpp"

void timeDisplayComponentRender(WatchState watchState, TftApi *tftApi)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
    tftApi->setCursor(10, 90);
    tftApi->setTextSize(1);
    tftApi->setTextFont(7);
    tftApi->print(buf);
}

Component createTimeDisplayComponent()
{
    return {
        .render = timeDisplayComponentRender,
    };
}