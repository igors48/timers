#pragma once

#include "core/app/app.hpp"

typedef Tiler_ Tiler;

typedef void (*SetApp)(App *app);
typedef void (*RenderApp)(bool forced);
typedef Component* (*Contains)(Tiler *tiler, signed short x, signed short y);

typedef struct Tiler_
{
    SetApp setApp;
    RenderApp renderApp;  
    Contains contains;
    TouchEventHandler onTouch; 
    TouchEventHandler onMove;  
    TouchEventHandler onRelease;  
    GestureEventHandler onGesture;  
} ;

Tiler createTiler(TftApi *tftApi);