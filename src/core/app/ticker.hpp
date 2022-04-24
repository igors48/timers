#pragma once

#include "core/tools/func.hpp"

typedef struct {
    unsigned int delayMs;
    VoidFunc func;
} TickerParameters;