#pragma once

#include "core/app/app.hpp"
#include "core/app/manager.hpp"
#include "core/system/sound.hpp"
#include "core/component/factory.hpp"

typedef struct 
{
    void (*beep)();
    void (*onGesture)(Gesture gesture);    
} ToolsAppApi;

App createClockApp(SoundApi *soundApiRef, Manager *managerRef, Factory *factoryRef);

Component* createToolsAppTile(ToolsAppApi *toolsAppApi, Factory *factory);
