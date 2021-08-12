#pragma once

#include "../watch/watch.hpp"
#include "../system/system.hpp"
#include "../watch/rtc.hpp"
#include "../tools/func.hpp"

typedef struct
{
    void *watchMutex;
    long *lastUserEventTimestamp;
    long goToSleepTime;
    Func goToSleep;
    void **tasks;
    int tasksCount; // todo unsigned char instead int
    SystemApi *systemApi;
    WatchApi *watchApi;
    RtcApi *rtcApi;
} SupervisorParameters;

void supervisor(SupervisorParameters *p);
void goToSleep(void *p);
