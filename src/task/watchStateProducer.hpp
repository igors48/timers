#pragma once

#include "../system/system.hpp"
#include "../watch/rtc.hpp"
#include "../watch/power.hpp"
#include "watchState.hpp"

typedef struct
{
    void *watchMutex;
    WatchState *state;
    RtcApi *rtcApi;
    SystemApi *systemApi;
    PowerApi *powerApi;
} WatchStateProducerParameters;

void watchStateProducer(void *v);