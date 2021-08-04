#include <stdio.h>

#include "watchStateRender.hpp"

void watchStateRender(void *v)
{
    WatchStateRenderParameters *p = (WatchStateRenderParameters *)v;
    if (p->systemApi->take(p->watchMutex, 10))
    {
        WatchState watchState = *p->state;

        char buf[16];

        for (int i = 0; i < p->componentsCount; i++)
        {
            Component current = p->components[i];
            current.render(current.state, watchState, p->tftApi);
        }

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