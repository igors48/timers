#pragma once

#include "core/app/app.hpp"

typedef struct {
    void *backgroundTaskHandle;
} ClockAppState;

void clockAppTick();

App createClockApp(RtcApi *rtcApiRef, Tiler *tilerRef);