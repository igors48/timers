#include <stdio.h>

#include "watchStateRender.hpp"

void watchStateRender(void *v)
{
    WatchStateRenderParameters *p = (WatchStateRenderParameters *)v;
    if (p->systemApi->take(p->watchMutex, 10))
    {
        WatchState watchState = *p->state;

        char buf[16];
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
        p->tftApi->setCursor(10, 90);
        p->tftApi->setTextSize(1);
        p->tftApi->setTextFont(7);
        p->tftApi->print(buf);

        p->tftApi->setCursor(100, 150);
        p->tftApi->setTextFont(1);
        p->tftApi->setTextSize(2);
        snprintf(buf, sizeof(buf), "%02d %%", watchState.battPercentage);
        p->tftApi->print(buf);

        p->tftApi->setCursor(0, 200);
        p->tftApi->setTextFont(1);
        p->tftApi->setTextSize(2);
        snprintf(buf, sizeof(buf), "x:%03d, y:%03d", watchState.touchX, watchState.touchY);
        p->tftApi->print(buf);

        p->systemApi->give(p->watchMutex);
    }
}