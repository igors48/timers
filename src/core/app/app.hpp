#pragma once

#include "core/system/system.hpp"
#include "core/component/group.hpp"

typedef struct App_ App;

typedef void (*AppEventHandler)(App *app);
typedef Component* (*AppGetActiveTile)();

typedef struct App_
{
    AppEventHandler activate;
    AppEventHandler deactivate;
    AppGetActiveTile getActiveTile; // todo is it used?
    SystemApi *systemApi; // todo is it used? 
    void *state; // todo is it used?
} App;