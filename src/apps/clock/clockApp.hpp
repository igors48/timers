#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/app/manager.hpp"

#include "core/watch/rtc.hpp"
#include "core/watch/power.hpp"
#include "core/watch/bma.hpp"

#include "core/component/factory.hpp"

typedef struct {
    Date (*getDate)();
    void (*adjDate)(signed char hourDelta);
    int (*getBattery)();
    unsigned int (*getStepCounter)();
    void (*onGesture)(Gesture gesture);
} ClockAppApi;

void clockAppTick();

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, BmaApi *bmaApiRef, Tiler *tilerRef, Manager *managerRef, Factory *factory);

Component* createClockAppTile(ClockAppApi *clockAppApi);

Component* createClockAppSetTimeTile(ClockAppApi *clockAppApi, Factory *factory);