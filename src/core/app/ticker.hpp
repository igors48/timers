#pragma once

#include "core/system/system.hpp"

typedef struct {
    void *watchMutex;
    unsigned int delayMs;
    void (*func)();
    SystemApi *systemApi;
} TickerParameters;

void tickerTask(void *p);
