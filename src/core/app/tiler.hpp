#pragma once

#include "core/app/app.hpp"

typedef void (*SetApp)(App *app);
typedef void (*RenderApp)(bool forced);

typedef struct
{
    SetApp setApp;
    RenderApp renderApp;    
} Tiler;

Tiler createTiler(TftApi *tftApi);