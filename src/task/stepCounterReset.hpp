#pragma once

#include "system/system.hpp"
#include "watch/rtc.hpp"
#include "watch/bma.hpp"

typedef struct 
{
    void *watchMutex;
    unsigned char lastReset;
    RtcApi *rtcApi;
    BmaApi *bmaApi;
    SystemApi *systemApi;
} StepCounterResetParameters;

void stepCounterReset(StepCounterResetParameters *p);
