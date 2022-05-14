#include <LilyGoWatch.h>

#include "stepperComponent.hpp"
#include "buttonComponent.hpp"
#include "group.hpp"

void stepperOnPlus(void *context)
{
    StepperComponentState* state = (StepperComponentState *)context;
    state->value++;
    state->onChange(state->value);
}

void stepperOnMinus(void *context)
{
    StepperComponentState* state = (StepperComponentState *)context;
    state->value--;
    state->onChange(state->value);
}
