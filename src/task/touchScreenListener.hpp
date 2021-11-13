#pragma once

#include "../watch/watch.hpp"
#include "../system/system.hpp"
#include "../component/component.hpp"

typedef struct {
    Component *target;
    signed short lastX;
    signed short lastY;
    void *watchMutex;
    long *lastUserEventTimestamp;
    Component *screen;
    WatchApi *watchApi;
    SystemApi *systemApi;
} TouchScreenListenerParameters;

void touchScreenListener(void *v);