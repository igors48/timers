#pragma once

#include "tools/func.hpp"

// no watch or freertos specific here. we should be able to use it in the native env

typedef struct
{
    VoidFunc readIRQ;
    BoolFunc isPEKShortPressIRQ;
    VoidFunc clearIRQ;
} PowerApi;

// took watch from package name
PowerApi watchPowerApi();