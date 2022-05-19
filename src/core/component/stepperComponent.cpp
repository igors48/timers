#include "stepperComponent.hpp"
#include "buttonComponent.hpp"
#include "group.hpp"

static void updateButtonsMode(StepperComponentState *state)
{
    ButtonComponentState *plusButtonState = (ButtonComponentState *)(state->plusButton->state); // todo fix this long way
    ButtonComponentState *minusButtonState = (ButtonComponentState *)(state->minusButton->state);
    plusButtonState->mode = BM_ENABLED;
    minusButtonState->mode = BM_ENABLED;
    if (state->value == state->max)
    {
        plusButtonState->mode = BM_DISABLED;        
    }
    if (state->value == state->min)
    {        
        minusButtonState->mode = BM_DISABLED;
    }
}

void stepperOnPlus(void *context)
{
    StepperComponentState *state = (StepperComponentState *)context;
    if (state->value == state->max)
    {
        return;
    }
    state->value++;
    updateButtonsMode(state);
    state->onChange(state->value);
}

void stepperOnMinus(void *context)
{
    StepperComponentState *state = (StepperComponentState *)context;
    if (state->value == state->min)
    {
        return;
    }
    state->value--;
    updateButtonsMode(state);
    state->onChange(state->value);
}

void stepperSetValue(void *context, signed short value)
{
    StepperComponentState *state = (StepperComponentState *)context;
    state->value = value;    
    updateButtonsMode(state);
}
