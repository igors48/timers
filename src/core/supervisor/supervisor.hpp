#pragma once

#include "core/watch/watch.hpp"
#include "core/system/system.hpp"
#include "core/watch/rtc.hpp"
#include "core/tools/func.hpp"
#include "core/app/manager.hpp"

typedef struct
{
    void *watchMutex;
    long *lastUserEventTimestamp;
    long goToSleepTime;
    void (*supervisorSleep)(void *v, unsigned int sleepTimeSec);
    SystemApi *systemApi;
    WatchApi *watchApi;
    RtcApi *rtcApi;
    Manager *manager;
} SupervisorParameters;

void supervisor(SupervisorParameters *p);
void supervisorSleep(void *p, unsigned int sleepTimeSec);
