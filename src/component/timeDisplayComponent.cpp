#include <stdio.h>

#include "component.hpp"
#include "timeDisplayComponent.hpp"

void timeDisplayComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextSize(1);
    tftApi->setTextFont(7);
    tftApi->print(buf);
}

Component createTimeDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .render = timeDisplayComponentRender,
    };
}