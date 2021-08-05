#pragma once

#include "../watch/watch.hpp"
#include "../system/system.hpp"

typedef void (*OnScreenTouch)(signed short x, signed short y);

typedef struct {
    bool touched;
    signed short firstX;
    signed short firstY;
    signed short lastX;
    signed short lastY;
    void *watchMutex;
    long *lastUserEventTimestamp;
    OnScreenTouch onScreenTouch;
    WatchApi *watchApi;
    SystemApi *systemApi;
} TouchScreenListenerParameters;

void touchScreenListener(void *v);