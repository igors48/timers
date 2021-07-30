#pragma once

#include "../watch/watch.hpp"

typedef struct {
    WatchApi *watchApi;
} TouchScreenListenerParameters;

void touchScreenListener(void *v);