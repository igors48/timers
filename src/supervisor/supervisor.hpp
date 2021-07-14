#pragma once

#include "../system/system.hpp"
#include "task.hpp"

typedef struct
{
    void *actionMutex;
    bool *action;
    void *lastEventTimestampMutex;
    long *lastEventTimestamp;
    long wakeUpTime;
    Func wakeUp;
    long goToSleepTime;
    Func goToSleep;
    TaskParameters *actionModeTasks[10];
    int actionModeTasksCount;
    TaskParameters *sleepModeTasks[10];
    int sleepModeTasksCount;
    SystemApi *systemApi;
} SupervisorParameters;


void supervisor(SupervisorParameters *p);
