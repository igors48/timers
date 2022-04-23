#pragma once

#include "core/system/system.hpp"
#include "core/component/group.hpp"

typedef struct App_ App;

typedef void (*AppEventHandler)(App *app);
typedef Component* (*AppGetTile)();

typedef struct App_
{
    AppEventHandler activate;
    AppEventHandler deactivate;
    AppGetTile getActiveTile;
    AppGetTile getPrevTile;
    AppGetTile getNextTile;
    SystemApi *systemApi; 
    void *state;
} App;