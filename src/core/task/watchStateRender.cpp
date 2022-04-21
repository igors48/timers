#include "watchStateRender.hpp"

static const char WATCH_STATE_RENDER[] = "watchStateRender";

// todo tests
void watchStateRender(void *v)
{
    WatchStateRenderParameters *p = (WatchStateRenderParameters *)v;
    if ((p->systemApi->take)(p->watchMutex, 10))
    {
        WatchState watchState = *p->state;
        Component *screen = p->screen;
        (screen->render)(screen, false, &watchState, p->tftApi);
        (p->systemApi->give)(p->watchMutex);
    }
        else
    {
        (p->systemApi->log)(WATCH_STATE_RENDER, "failed to take watch mutex");
    }
}