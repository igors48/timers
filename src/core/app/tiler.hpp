#pragma once

#include "core/app/app.hpp"

typedef struct 
{
    void (*setApp)(App *app);
    void (*renderApp)(bool forced);  
    Component* (*contains)(signed short x, signed short y);
    void (*onTouch)(Component *component, signed short x, signed short y, unsigned long tickCount); 
    void (*onMove)(Component *component, signed short x, signed short y, unsigned long tickCount); 
    void (*onRelease)(Component *component, signed short x, signed short y, unsigned long tickCount); 
    void (*onGesture)(Gesture gesture); 
    void (*onButton)();
} Tiler;

Tiler createTiler(TftApi *tftApi);