#pragma once

#include "../system/system.hpp"
#include "../watch/tft.hpp"
#include "../component/component.hpp"

#include "watchState.hpp"

typedef struct
{
    void *watchMutex;
    WatchState *state;
    SystemApi *systemApi;
    TftApi *tftApi;
    Component *components;
    unsigned char componentsCount;
} WatchStateRenderParameters;

void watchStateRender(void *v);