#pragma once

#include "core/system/system.hpp"
#include "core/watch/power.hpp"

typedef struct
{
    void *watchMutex;
    long *lastUserEventTimestamp;
    PowerApi *powerApi;
    SystemApi *systemApi;
} ButtonListenerParameters;

void buttonListener(ButtonListenerParameters *p);

