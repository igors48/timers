#pragma once

#include "system/system.hpp"
#include "watch/power.hpp"

typedef struct
{
    void *lastShortPressTimestampMutex;
    long *lastShortPressTimestamp;
    PowerApi *powerApi;
    SystemApi *systemApi;
} ButtonListenerParameters;

void buttonListener(ButtonListenerParameters *p);

