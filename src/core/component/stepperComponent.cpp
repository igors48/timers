#include "stepperComponent.hpp"
#include "buttonComponent.hpp"
#include "group.hpp"

#include <LilyGoWatch.h>

static char PLUS[] = "+";
static char MINUS[] = "-";

static void onPlus(void *context)
{
    StepperComponentState* state = (StepperComponentState *)context;
    state->value++;
    state->onChange(state->value);
    Serial.println("plus");
}

static void onMinus(void *context)
{
    StepperComponentState* state = (StepperComponentState *)context;
    state->value--;
    state->onChange(state->value);
    Serial.println("minus");
}

StepperComponentState* createStepperComponentStateRef(signed short min, signed short max, signed short value, OnStepperChange onChange)
{
    ButtonComponentState *plusButtonState = createButtonStateRef(PLUS, EG_REPEAT, onPlus);
    ButtonComponentState *minusButtonState = createButtonStateRef(MINUS, EG_REPEAT, onMinus);

    Component plusButton = createButtonComponent(0, 0, 50, 50, plusButtonState);
    Component minusButton = createButtonComponent(0, 55, 50, 50, minusButtonState);

    StepperComponentState *state = (StepperComponentState *)pvPortMalloc(sizeof(StepperComponentState));

    state->min = 0;
    state->max = 10;
    state->value = 5;
    state->plusButton = plusButton;
    state->minusButton = minusButton;
    state->onChange = onChange;

    plusButtonState->context = (void *)state;
    minusButtonState->context = (void *)state;

    return state;
}

Component* createStepperComponentRef(signed short x, signed short y, StepperComponentState *state)
{
    void** components = (void**)pvPortMalloc(2 * sizeof(void*));
    components[0] = &(state->plusButton);
    components[1] = &(state->minusButton);

    GroupState* groupState = createGroupStateRef(2, components);
    return createGroupComponentRef(0, 0, groupState);
}