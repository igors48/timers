#pragma once

#include "../system/system.hpp"
#include "../watch/rtc.hpp"
#include "watchState.hpp"

typedef struct
{
    void *stateMutex;
    WatchState *state;
    RtcApi *rtcApi;
    SystemApi *systemApi;
} WatchStateProducerParameters;

void watchStateProducer(void *v);