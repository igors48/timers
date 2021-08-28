#pragma once

#include "component.hpp"

typedef struct
{
    int _stepCount;
} StepCounterComponentState;

Component createStepCounterComponent(signed short x, signed short y, signed short w, signed short h, StepCounterComponentState *state);