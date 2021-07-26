#pragma once

#include "../system/system.hpp"
#include "watch/ttgo.hpp"

#include "watchState.hpp"

typedef struct
{
    void *stateMutex;
    WatchState *state;
    SystemApi *systemApi;
    TTGOClass *watch;
} WatchStateRenderParameters;

void watchStateRender(void *v);