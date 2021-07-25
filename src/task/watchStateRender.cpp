#include "watchStateRender.hpp"

void watchStateRender(WatchStateRenderParameters *p)
{
    if (p->systemApi->take(p->watchStateMutex, 10))
    {
        WatchState watchState = *p->state;
        p->systemApi->give(p->watchStateMutex);

        char buf[256];
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
        p->watch->tft->setCursor(25, 90);
        p->watch->tft->setTextSize(4);
        p->watch->tft->setTextFont(1);
        p->watch->tft->print(buf);
    }
}