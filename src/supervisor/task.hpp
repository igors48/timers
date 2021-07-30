#pragma once

#include "../system/system.hpp"
#include "../tools/func.hpp"

typedef struct
{
    void *handle;
    Func func; 
    void *parameters;
    // todo make this mutex common for all struct access not only for termination
    void *terminationMutex; // individual for each tasks  
    bool termination; // guarded by terminationMutex
    bool canBeSuspended; // guarded by terminationMutex
    unsigned int taskDelay;
    SystemApi *systemApi;
} TaskParameters;

void task(void *p);