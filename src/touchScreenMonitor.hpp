#pragma once

#include <freertos.hpp>
#include <twatch.hpp>

typedef struct
{
    void *lastTouchTimestampMutex;
    long *lastTouchTimestamp;
    GetTouched getTouched;
    Time time;
    Take take;
    Give give;
    Log log;
} TouchScreenMonitorParameters;

void touchScreenMonitor(TouchScreenMonitorParameters *p);
