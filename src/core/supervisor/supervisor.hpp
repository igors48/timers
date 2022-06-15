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
    WakeUpReason (*supervisorSleep)(void *v, unsigned long sleepTimeSec);
    SystemApi *systemApi;
    WatchApi *watchApi;
    RtcApi *rtcApi;
    Manager *manager;
} SupervisorParameters;

typedef struct 
{
    unsigned long (*getNextWakeUpPeriod)();
    unsigned long (*getTimeToSleep)();
    WakeUpReason (*getWakeUpReason)();
} SupervisorApi;

void supervisor(SupervisorParameters *p);
WakeUpReason supervisorSleep(void *p, unsigned long sleepTimeMillis);

SupervisorApi watchSupervisorApi();