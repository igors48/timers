#pragma once

#include "core/component/component.hpp"

typedef void (*OnStepperChange)(signed short value);

typedef struct 
{
    signed short min;
    signed short max;
    signed short value;
    Component* plusButton;
    Component* minusButton;
    OnStepperChange onChange;
} StepperComponentState;

void stepperOnPlus(void *context);
void stepperOnMinus(void *context);
