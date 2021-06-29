#pragma once

#include "freertos.hpp"
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
    Time time;
    Take take;
    Give give;
    Log log;
} SupervisorParameters;


void supervisor(SupervisorParameters *p);
