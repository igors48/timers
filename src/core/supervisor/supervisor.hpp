#pragma once

#include "core/watch/watch.hpp"
#include "core/system/system.hpp"
#include "core/watch/rtc.hpp"
#include "core/tools/func.hpp"

typedef void (*SupervisorSleep)(void *v, unsigned short sleepTimeSec);

typedef struct
{
    void *watchMutex;
    long *lastUserEventTimestamp;
    long goToSleepTime;
    SupervisorSleep supervisorSleep;
    void **tasks;
    int tasksCount; // todo unsigned char instead int
    SystemApi *systemApi;
    WatchApi *watchApi;
    RtcApi *rtcApi;
} SupervisorParameters;

void supervisor(SupervisorParameters *p);
void supervisorSleep(void *p, unsigned short sleepTimeSec);
