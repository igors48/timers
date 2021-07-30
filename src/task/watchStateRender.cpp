#include <stdio.h>

#include "watchStateRender.hpp"

void watchStateRender(void *v)
{
    WatchStateRenderParameters *p = (WatchStateRenderParameters *)v;
    if (p->systemApi->take(p->stateMutex, 10))
    {
        WatchState watchState = *p->state;
        p->systemApi->give(p->stateMutex);

        char buf[16];
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
        p->tftApi->setCursor(25, 90);
        p->tftApi->setTextSize(4);
        p->tftApi->setTextFont(1);
        p->tftApi->print(buf);

        p->tftApi->setCursor(100, 130);
        p->tftApi->setTextSize(2);
        snprintf(buf, sizeof(buf), "%02d %%", watchState.battPercentage);
        p->tftApi->print(buf);
        p->tftApi->print(" %");
    }
}