#pragma once

#include "../watch/watch.hpp"
#include "../system/system.hpp"
#include "../watch/rtc.hpp"
#include "../tools/func.hpp"

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
