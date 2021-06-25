#pragma once

#include "freertos.hpp"

typedef void (*Func)(void *p);

typedef struct
{
    void *handle;
    Func func;
    void *parameters;
    void *actionMutex;
    bool *action;
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
