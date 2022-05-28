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
    unsigned short goToSleepTime;
    WakeUpReason (*supervisorSleep)(void *v, unsigned int sleepTimeSec);
    SystemApi *systemApi;
    WatchApi *watchApi;
    RtcApi *rtcApi;
    Manager *manager;
} SupervisorParameters;

typedef struct 
{
    unsigned int (*getNextWakeUpPeriod)();
    unsigned short (*getTimeToSleep)();
    WakeUpReason (*getWakeUpReason)();
} SupervisorApi;

void supervisor(SupervisorParameters *p);
WakeUpReason supervisorSleep(void *p, unsigned int sleepTimeSec);

SupervisorApi watchSupervisorApi();