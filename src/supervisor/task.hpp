#pragma once

#include "../system/system.hpp"
#include "../tools/func.hpp"

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
    SystemApi *systemApi;
} TaskParameters;

//void task(void *p);

//TaskParameters create(Func func, void *parameters, void *mutex, unsigned int taskDelay);
