#pragma once

#include "core/app/app.hpp"
#include "core/app/tiler.hpp"
#include "core/system/system.hpp"
#include "core/watch/bma.hpp"
#include "core/watch/bma.hpp"

typedef struct {
    unsigned int (*getStepCounter)();
    void (*resetStepCounter)();
    void (*onGesture)(Gesture gesture);
    void (*render)(bool forced);
} StepAppApi;

void stepAppTick();

App createStepApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, BmaApi *bmaApi, Tiler *tilerRef); 

Component* createStepAppTile(StepAppApi *stepAppApi);