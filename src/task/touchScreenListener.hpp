#pragma once

#include "../watch/watch.hpp"
#include "../system/system.hpp"
#include "../component/component.hpp"

typedef void (*OnScreenTouch)(signed short x, signed short y);

typedef Component* (*FindTarget)(signed short x, signed short y);

typedef struct {
    Component *target;
    signed short firstX;
    signed short firstY;
    signed short lastX;
    signed short lastY;
    void *watchMutex;
    long *lastUserEventTimestamp;
    FindTarget findTarget;
    WatchApi *watchApi;
    SystemApi *systemApi;
} TouchScreenListenerParameters;

void touchScreenListener(void *v);