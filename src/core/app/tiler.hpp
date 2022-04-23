#pragma once

#include "core/app/app.hpp"

typedef void (*SetApp)(App *app);
typedef void (*ActivatePrevTile)();
typedef void (*ActivateNextTile)();
typedef void (*RenderTile)();

typedef struct
{
    SetApp setApp;
    ActivatePrevTile activatePrevTile;
    ActivateNextTile activateNextTile;
    RenderTile renderTile;    
} Tiler;

Tiler createTiler(TftApi *tftApi);