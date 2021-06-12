#pragma once

#include "freertos.hpp"

typedef void (*Func)(void *p);

typedef struct
{
    Func func;
    void *parameters;
    void *mutex;
    bool termination;
    bool canBeSuspended;
    unsigned int taskDelay;
    Take take;
    Give give;
    Delay delay;
    Log log;
} TaskParameters;

