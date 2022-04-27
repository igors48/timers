#pragma once

#include "core/app/app.hpp"

typedef struct Tiler_ Tiler;

typedef void (*SetApp)(App *app);
typedef void (*RenderApp)(bool forced);
typedef Component* (*ContainsTiler)(Tiler *tiler, signed short x, signed short y);

struct Tiler_
{
    SetApp setApp;
    RenderApp renderApp;  
    ContainsTiler contains;
    TouchEventHandler onTouch; 
    TouchEventHandler onMove;  
    TouchEventHandler onRelease;  
    GestureEventHandler onGesture;  
};

Tiler createTiler(TftApi *tftApi);