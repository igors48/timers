#include <stdio.h>

#include "component.hpp"
#include "touchDisplayComponent.hpp"

void touchDisplayComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "x:%03d, y:%03d", watchState.touchX, watchState.touchY);
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextFont(1);
    tftApi->setTextSize(2);
    tftApi->setTextColor(0xFFFF);
    tftApi->print(buf);
}

Component createTouchDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .state = NULL,
        .render = touchDisplayComponentRender,
        .onTouch = componentOnTouch,
    };
}