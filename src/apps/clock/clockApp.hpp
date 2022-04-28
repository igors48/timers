#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/watch/rtc.hpp"
#include "core/watch/power.hpp"

typedef void (*OnGesture)(Gesture gesture);

typedef struct {
    void *backgroundTaskHandle;
} ClockAppState;

void clockAppTick();

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, Tiler *tilerRef);