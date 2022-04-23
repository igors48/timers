#pragma once

#include "core/app/app.hpp"

typedef struct {
    void *backgroundTaskHandle;
} ClockAppState;

App createClockApp();