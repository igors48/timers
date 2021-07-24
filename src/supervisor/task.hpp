#pragma once

#include "../system/system.hpp"
#include "../tools/func.hpp"

typedef struct
{
    void *handle;
    Func func; // todo consider approach - create pool of such structs. just replace func and parameters during runtime
    void *parameters;
    void *terminationMutex; // individual for each tasks
    bool termination; // guarded by terminationMutex
    bool canBeSuspended; // guarded by terminationMutex
    unsigned int taskDelay;
    SystemApi *systemApi;
} TaskParameters;

void task(void *p);