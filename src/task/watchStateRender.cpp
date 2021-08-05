#include <stdio.h>

#include "watchStateRender.hpp"

void watchStateRender(void *v)
{
    WatchStateRenderParameters *p = (WatchStateRenderParameters *)v;
    if (p->systemApi->take(p->watchMutex, 10))
    {
        WatchState watchState = *p->state;

        for (int i = 0; i < p->componentsCount; i++)
        {
            Component current = p->components[i];
            current.render(current.state, watchState, p->tftApi);
        }

        p->systemApi->give(p->watchMutex);
    }
}