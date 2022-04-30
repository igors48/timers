#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/app/manager.hpp"

#include "core/watch/rtc.hpp"
#include "core/watch/power.hpp"

typedef struct {
    Date (*getDate)();
    void (*adjDate)(signed char hourDelta);
    int (*getBattery)();
    void (*onGesture)(Gesture gesture);
    void (*render)(bool forced);
} ClockAppApi;

void clockAppTick();

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, Tiler *tilerRef, Manager *managerRef);