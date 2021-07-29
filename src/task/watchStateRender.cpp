#include "watchStateRender.hpp"

void watchStateRender(void *v)
{
    WatchStateRenderParameters *p = (WatchStateRenderParameters *)v;
    if (p->systemApi->take(p->stateMutex, 10))
    {
        WatchState watchState = *p->state;
        p->systemApi->give(p->stateMutex);

        char buf[256];
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", watchState.date.hour, watchState.date.minute, watchState.date.second);
        p->watch->tft->setCursor(25, 90);
        p->watch->tft->setTextSize(4);
        p->watch->tft->setTextFont(1);
        p->watch->tft->print(buf);

        p->watch->tft->setCursor(100, 130);
        p->watch->tft->setTextSize(2);
        p->watch->tft->print(watchState.battPercentage);
        p->watch->tft->print(" %");
    }
}