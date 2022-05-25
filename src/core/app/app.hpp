#pragma once

#include <climits>

#include "core/system/system.hpp"
#include "core/component/group.hpp"

const unsigned int NW_NO_SLEEP = 0;
const unsigned int NW_DONT_CARE = UINT_MAX;

typedef struct App_
{
    void (*activate)();
    void (*deactivate)();
    Component* (*getActiveTile)();
    unsigned int (*getNextWakeUpPeriod)();
} App;