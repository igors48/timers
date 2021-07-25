#pragma once

#include "../system/system.hpp"
#include "../watch/rtc.hpp"
#include "watchState.hpp"

typedef struct
{
    void *watchStateMutex;
    WatchState *state;
    RtcApi *rtcApi;
    SystemApi *systemApi;
} WatchStateProducerParameters;

void watchStateProducer(WatchStateProducerParameters *p);