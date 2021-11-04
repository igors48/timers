#include "watchStateRender.hpp"

static const char WATCH_STATE_RENDER[] = "watchStateRender";

// todo tests
void watchStateRender(void *v)
{
    WatchStateRenderParameters *p = (WatchStateRenderParameters *)v;
    if (p->systemApi->take(p->watchMutex, 10))
    {
        WatchState watchState = *p->state;
        for (int i = 0; i < p->componentsCount; i++)
        {
            Component current = p->components[i];
            bool needRender = current.newState(&current, &watchState);
            if (needRender)
            {
                current.render(0, 0, &current, &watchState, p->tftApi);
            }
        }
        p->systemApi->give(p->watchMutex);
    }
        else
    {
        p->systemApi->log(WATCH_STATE_RENDER, "failed to take watch mutex");
    }
}