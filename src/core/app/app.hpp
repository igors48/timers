#pragma once

#include <climits>

#include "core/system/system.hpp"
#include "core/component/group.hpp"

const unsigned int NW_NO_SLEEP = 0;
const unsigned int NW_DONT_CARE = UINT_MAX;//4294967295;

typedef struct App_ App; // todo remove after remove parameter (see below)

typedef struct App_
{
    void (*activate)(App *app); // todo remove parameter
    void (*deactivate)(App *app); // todo remove parameter
    Component* (*getActiveTile)();
    unsigned int (*getNextWakeUpPeriod)();
} App;