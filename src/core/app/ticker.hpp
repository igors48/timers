#pragma once

#include "core/tools/func.hpp"
#include "core/system/system.hpp"

typedef struct {
    void *watchMutex;
    unsigned int delayMs;
    VoidFunc func;
    SystemApi *systemApi;
} TickerParameters;

void tickerTask(void *p);
