#pragma once

#include "../watch/watch.hpp"
#include "../system/system.hpp"
#include "../tools/func.hpp"
#include "task.hpp"

typedef struct
{
    void *lastEventTimestampMutex;
    long *lastEventTimestamp;
    long goToSleepTime;
    Func goToSleep;
    TaskParameters **tasks;
    int tasksCount;
    SystemApi *systemApi;
    WatchApi *watchApi;
} SupervisorParameters;

void supervisor(SupervisorParameters *p);
void goToSleep(void *p);
