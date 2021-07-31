#pragma once

#include "../watch/watch.hpp"

typedef struct {
    bool touched;
    signed short firstX;
    signed short firstY;
    signed short lastX;
    signed short lastY;
    WatchApi *watchApi;
} TouchScreenListenerParameters;

void touchScreenListener(void *v);