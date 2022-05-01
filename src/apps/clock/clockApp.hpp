#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/app/manager.hpp"

#include "core/watch/rtc.hpp"
#include "core/watch/power.hpp"
#include "core/watch/bma.hpp"

typedef struct {
    Date (*getDate)();
    void (*adjDate)(signed char hourDelta);
    int (*getBattery)();
    unsigned int (*getStepCounter)();
    void (*onGesture)(Gesture gesture);
} ClockAppApi;

void clockAppTick();

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, BmaApi *bmaApiRef, Tiler *tilerRef, Manager *managerRef);

Component* createClockAppTile(ClockAppApi *clockAppApi);

Component* createClockAppSetTimeTile(ClockAppApi *clockAppApi);