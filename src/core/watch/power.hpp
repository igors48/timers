#pragma once

#include "core/tools/func.hpp"

// no watch or freertos specific here. we should be able to use it in the native env

typedef struct
{
    VoidFunc readIRQ;
    BoolFunc isPEKShortPressIRQ;
    VoidFunc clearIRQ;
    IntFunc getBattPercentage;
} PowerApi;

// took watch from package name
PowerApi watchPowerApi();