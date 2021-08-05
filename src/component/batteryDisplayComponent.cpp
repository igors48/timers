#include <stdio.h>

#include "component.hpp"
#include "batteryDisplayComponent.hpp"

void batteryDisplayComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d %%", watchState.battPercentage);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextFont(1);
    tftApi->setTextSize(2);
    tftApi->print(buf);
}

Component createBatteryDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .render = batteryDisplayComponentRender,
    };
}