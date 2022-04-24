#pragma once

#include "core/app/app.hpp"

typedef void (*SetApp)(App *app);
typedef void (*RenderApp)(bool forced);

typedef struct
{
    SetApp setApp;
    RenderApp renderApp;  
    Contains contains;
    TouchEventHandler onTouch; 
    TouchEventHandler onMove;  
    TouchEventHandler onRelease;  
    GestureEventHandler onGesture;  
} Tiler;

Tiler createTiler(TftApi *tftApi);