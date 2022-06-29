#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/app/manager.hpp"

#include "core/system/system.hpp"

#include "core/watch/bma.hpp"
#include "core/watch/bma.hpp"

#include "core/component/factory.hpp"

typedef struct {
    unsigned int (*getStepCounter)();
    void (*resetStepCounter)();
    void (*onGesture)(Gesture gesture);
} StepAppApi;

void stepAppTick(unsigned long tick);

App createStepApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, BmaApi *bmaApi, Tiler *tilerRef, Manager *managerRef, Factory *factory); 

Component* createStepAppTile(StepAppApi *stepAppApi, Factory *factory);