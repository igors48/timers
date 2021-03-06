#pragma once

#include "core/app/app.hpp"
#include "core/app/manager.hpp"
#include "core/system/sound.hpp"
#include "core/component/factory.hpp"

typedef struct 
{
    unsigned long (*getTimerState)();
    void (*startTimer)();
    void (*stopTimer)();
    void (*onGesture)(Gesture gesture);
} TimerAppApi;

void timerAppTick(unsigned long tick);

App createTimerApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, SoundApi *soundApiRef, Tiler *tilerRef, Manager *managerRef, Factory *factoryRef);

Component* createTimerAppTile(TimerAppApi *timerAppApi, Factory *factory);