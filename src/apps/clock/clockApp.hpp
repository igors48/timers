#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/watch/rtc.hpp"
#include "core/watch/power.hpp"

typedef struct {
    Date (*getDate)();
    void (*adjDate)(Date date);
    int (*getBattery)();
    void (*onGesture)(Gesture gesture);
    void (*render)(bool forced);
} ClockAppApi;

typedef struct {
    void *backgroundTaskHandle;
} ClockAppState;

void clockAppTick();

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, Tiler *tilerRef);