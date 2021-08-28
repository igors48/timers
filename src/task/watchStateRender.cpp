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
            bool needRender = current.newState(&current, &watchState);
            if (needRender)
            {
                current.render(&current, &watchState, p->tftApi);
            }
        }
        p->systemApi->give(p->watchMutex);
    }
    // todo tests
    // todo log message when mutex is busy
}