#pragma once

#include "core/app/app.hpp"

typedef struct {
    void *backgroundTaskHandle;
} ClockAppState;

void clockAppTick();

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, Tiler *tilerRef);