#pragma once

#include "freertos.hpp"

typedef void (*Func)(void *p);

typedef struct
{
    Func func;
    void *parameters;
    void *terminationMutex;
    bool termination;
    bool canBeSuspended;
    unsigned int taskDelay;
    Take take;
    Give give;
    Delay delay;
    Log log;
} TaskParameters;

void task(void *p);

TaskParameters create(Func func, void *parameters, void *mutex, unsigned int taskDelay);
