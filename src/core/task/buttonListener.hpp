#pragma once

#include "core/system/system.hpp"
#include "core/watch/power.hpp"
#include "core/app/tiler.hpp"

typedef struct
{
    void *watchMutex;
    long *lastUserEventTimestamp;
    Tiler *tiler;
    PowerApi *powerApi;
    SystemApi *systemApi;
} ButtonListenerParameters;

void buttonListener(ButtonListenerParameters *p);

