#pragma once

#include "../system/system.hpp"
#include "../watch/tft.hpp"

#include "watchState.hpp"

typedef struct
{
    void *stateMutex;
    WatchState *state;
    SystemApi *systemApi;
    TftApi *tftApi;
} WatchStateRenderParameters;

void watchStateRender(void *v);