#pragma once

#include "core/app/app.hpp"
#include "core/app/manager.hpp"
#include "core/system/sound.hpp"
#include "core/component/factory.hpp"

typedef struct 
{
} TimerAppApi;

void timerAppTick();

App createTimerApp(void *backgroundTaskHandleRef, SoundApi *soundApiRef, Manager *managerRef, Factory *factoryRef);

Component* createTimerAppTile(TimerAppApi *timerAppApi, Factory *factory);