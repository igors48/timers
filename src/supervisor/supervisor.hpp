#pragma once

#include "../system/system.hpp"
#include "../tools/func.hpp"
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
    TaskParameters **actionModeTasks;
    int actionModeTasksCount;
    TaskParameters **sleepModeTasks;
    int sleepModeTasksCount;
    SystemApi *systemApi;
} SupervisorParameters;

void supervisor(SupervisorParameters *p);
void goToSleep(void *p);
void wakeUp(void *p);