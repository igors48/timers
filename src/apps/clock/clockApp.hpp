#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/app/manager.hpp"

#include "core/watch/rtc.hpp"
#include "core/watch/power.hpp"
#include "core/watch/bma.hpp"
#include "core/supervisor/supervisor.hpp"

#include "core/component/factory.hpp"

typedef struct {
    Date (*getDate)();
    void (*setTime)(unsigned char hour, unsigned char minute);
    void (*setDate)(unsigned char day, unsigned char month);
    int (*getBattery)();
    unsigned int (*getStepCounter)();
    unsigned long (*getTimeToSleep)();
    unsigned long (*getNextWakeUpPeriod)();
    WakeUpReason (*getWakeUpReason)();
    void (*onGesture)(Gesture gesture);
    unsigned long (*getTickCount)();
} ClockAppApi;

void clockAppTick(unsigned long tick);

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, BmaApi *bmaApiRef, SupervisorApi *supervisorApiRef, Tiler *tilerRef, Manager *managerRef, Factory *factory);

Component* createClockAppTile(ClockAppApi *clockAppApi, Factory *factory);
Component* createClockAppSetTimeTile(ClockAppApi *clockAppApi, Factory *factory);
Component* createClockAppSetDateTile(ClockAppApi *clockAppApi, Factory *factory);