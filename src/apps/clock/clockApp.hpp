#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/watch/rtc.hpp"

typedef struct {
    void *backgroundTaskHandle;
} ClockAppState;

void clockAppTick();

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, Tiler *tilerRef);