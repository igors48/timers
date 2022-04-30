#pragma once

#include "core/system/system.hpp"
#include "core/component/group.hpp"

typedef struct App_ App;

typedef struct App_
{
    void (*activate)(App *app);
    void (*deactivate)(App *app);
    Component* (*getActiveTile)();
} App;