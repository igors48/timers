#pragma once

#include "system/system.hpp"
#include "watch/power.hpp"

typedef struct
{
    void *watchMutex;
    long *lastUserEventTimestamp;
    PowerApi *powerApi;
    SystemApi *systemApi;
} ButtonListenerParameters;

void buttonListener(ButtonListenerParameters *p);

