#pragma once

#include "../watch/watch.hpp"
#include "../system/system.hpp"
#include "../tools/func.hpp"

typedef struct
{
    void *watchMutex;
    long *lastEventTimestamp;
    long goToSleepTime;
    Func goToSleep;
    void **tasks;
    int tasksCount;
    SystemApi *systemApi;
    WatchApi *watchApi;
} SupervisorParameters;

void supervisor(SupervisorParameters *p);
void goToSleep(void *p);
