#pragma once

#include <climits>

#include "core/system/system.hpp"
#include "core/component/group.hpp"

const unsigned long NW_NO_SLEEP = 0;
const unsigned long NW_DONT_CARE = UINT_MAX;

typedef struct App_
{
    void (*activate)();
    void (*deactivate)();
    Component* (*getActiveTile)();
    unsigned long (*getNextWakeUpPeriod)();
} App;