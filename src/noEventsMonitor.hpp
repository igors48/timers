#pragma once

#include "freertos.hpp"
#include "twatch.hpp"

typedef struct
{
    void *lastTouchTimestampMutex;
    long *lastTouchTimestamp;
    void *backlightLevelMutex;
    unsigned char *backlightLevel;
    Sleep sleep;
    Take take;
    Give give;
    Time time;
    Log log;

} NoEventsMonitorParameters;

void noEventsMonitor(NoEventsMonitorParameters *p);