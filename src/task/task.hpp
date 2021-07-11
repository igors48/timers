#pragma once

#include "freertos.hpp"

typedef void (*Func)(void *p);

typedef struct
{
    void *handle;
    Func func;
    void *parameters;
    void *actionMutex; // common for all tasks
    bool *action;
    void *terminationMutex; // individual for each tasks
    bool termination; // guarded by terminationMutex
    bool canBeSuspended; // guarded by terminationMutex
    unsigned int taskDelay;
    Take take;
    Give give;
    Delay delay;
    Log log;
} TaskParameters;

void task(void *p);

TaskParameters create(Func func, void *parameters, void *mutex, unsigned int taskDelay);
