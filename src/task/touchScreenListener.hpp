#pragma once

#include "../watch/watch.hpp"

typedef void (*OnTouch)(signed short x, signed short y);

typedef struct {
    bool touched;
    signed short firstX;
    signed short firstY;
    signed short lastX;
    signed short lastY;
    OnTouch onTouch;
    WatchApi *watchApi;
} TouchScreenListenerParameters;

void touchScreenListener(void *v);