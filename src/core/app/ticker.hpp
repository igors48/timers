#pragma once

#include "core/system/system.hpp"

typedef struct {
    char *name;
    void *watchMutex;
    unsigned int delayMs;
    void (*func)();
    SystemApi *systemApi;
} TickerParameters;

void tickerTask(void *p);
