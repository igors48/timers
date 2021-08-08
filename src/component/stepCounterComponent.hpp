#pragma once

#include "component.hpp"

typedef struct
{
    int _stepCount;
} StepCounterComponentState;

Component createStepCounterComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, StepCounterComponentState *state);