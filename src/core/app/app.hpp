#pragma once

#include "core/system/system.hpp"

typedef struct App_ App;

typedef void (*AppEventHandler)(App *app);

typedef struct App_
{
    AppEventHandler activate;
    AppEventHandler deactivate;
    SystemApi *systemApi; 
    void *state;
} App;