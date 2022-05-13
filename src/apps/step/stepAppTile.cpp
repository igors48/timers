#include <stdio.h>

#include "stepApp.hpp"

#include "core/component/buttonComponent.hpp"
#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"
#include "core/component/stepperComponent.hpp"

static char RESET[] = "RESET";

static const unsigned char COMPONENTS_COUNT = 6;
static void *components[COMPONENTS_COUNT];

static TextState stepCounter;
static Component stepCounterComponent;

static ButtonComponentState resetButtonState;
static Component resetButton;

static GroupState state;
static Component group;

static StepAppApi *api;

signed short firstStepperValue;
signed short secondStepperValue;

static void provideStepCounterState(TextState *state)
{
    unsigned int stepCount = (api->getStepCounter)();
    snprintf(state->content, sizeof(state->content), "S:%05d", stepCount);
}

static void provideFirstStepperState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "%03d", firstStepperValue);
}

static void provideSecondStepperState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "%03d", secondStepperValue);
}

static void reset(void *context)
{
    (api->resetStepCounter)();
}

static void onGesture(Component *component, Gesture gesture)
{
    (api->onGesture)(gesture);
}

static void onFirstStepperChange(signed short value)
{
    firstStepperValue = value;
}

static void onSecondStepperChange(signed short value)
{
    secondStepperValue = value;
}

Component* createStepAppTile(StepAppApi* stepAppApi)
{
    api = stepAppApi;
    
    stepCounter = createTextState(1, 3, COLOR_ATTENTION, provideStepCounterState);
    resetButtonState = createButtonState(RESET, EG_ONCE, reset);

    stepCounterComponent = createTextComponent(55, 120, 50, 50, &stepCounter);
    resetButton = createButtonComponent(60, 195, 66, 25, &resetButtonState);

    StepperComponentState* firstStepperState = createStepperComponentStateRef(0, 10, 5, onFirstStepperChange);   
    Component* firstStepper = createStepperComponentRef(0, 0, firstStepperState);

    StepperComponentState* secondStepperState = createStepperComponentStateRef(0, 10, 5, onSecondStepperChange);   
    Component* secondStepper = createStepperComponentRef(190, 0, secondStepperState);

    TextState* firstStepperTextState = createTextStateRef(1, 3, COLOR_ATTENTION, provideFirstStepperState);
    Component* firstStepperText = createTextComponentRef(55, 10, 50, 50, firstStepperTextState);

    TextState* secondStepperTextState = createTextStateRef(1, 3, COLOR_ATTENTION, provideSecondStepperState);
    Component* secondStepperText = createTextComponentRef(120, 10, 50, 50, secondStepperTextState);

    components[0] = &stepCounterComponent;
    components[1] = &resetButton;
    components[2] = firstStepper;
    components[3] = firstStepperText;
    components[4] = secondStepper;
    components[5] = secondStepperText;
 
    state = createGroupState(COMPONENTS_COUNT, components);
    group = createGroupComponent(0, 0, &state);
    group.onGesture = onGesture; 

    group.mount(&group, 0, 0);

    return &group;
}