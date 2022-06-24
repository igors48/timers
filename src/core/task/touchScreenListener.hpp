#pragma once

#include "../watch/watch.hpp"
#include "../system/system.hpp"
#include "../component/component.hpp"
#include "core/app/tiler.hpp"

typedef struct {
    Component *target; // todo listener is a singleton - state var can be moved to cpp file #106
    signed short firstX;
    signed short firstY;
    signed short lastX;
    signed short lastY;
    void *watchMutex;
    long *lastUserEventTimestamp;
    Tiler *tiler;
    WatchApi *watchApi;
    SystemApi *systemApi;
} TouchScreenListenerParameters;

void touchScreenListener(void *v);