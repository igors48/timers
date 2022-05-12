#include <stdio.h>

#include <LilyGoWatch.h>

#include "stepApp.hpp"

#include "core/component/buttonComponent.hpp"
#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"
#include "core/component/stepperComponent.hpp"

static char RESET[] = "RESET";

static const unsigned char COMPONENTS_COUNT = 4;
static void *components[COMPONENTS_COUNT];

static TextState stepCounter;
static Component stepCounterComponent;

static ButtonComponentState resetButtonState;
static Component resetButton;

static GroupState state;
static Component group;

static StepAppApi *api;

static void provideStepCounterState(TextState *state)
{
    unsigned int stepCount = (api->getStepCounter)();
    snprintf(state->content, sizeof(state->content), "S:%05d", stepCount);
}

static void reset(void *context)
{
    (api->resetStepCounter)();
}

static void onGesture(Component *component, Gesture gesture)
{
    (api->onGesture)(gesture);
}

static void onStepperChange(signed short value)
{
    Serial.printf("stepper %d\r\n", value);
}

Component* createStepAppTile(StepAppApi* stepAppApi)
{
    api = stepAppApi;
    
    stepCounter = createTextState(1, 3, COLOR_ATTENTION, provideStepCounterState);
    resetButtonState = createButtonState(RESET, EG_ONCE, reset);

    stepCounterComponent = createTextComponent(55, 120, 50, 50, &stepCounter);
    resetButton = createButtonComponent(60, 195, 66, 25, &resetButtonState);

    StepperComponentState* stepperState = createStepperComponentStateRef(0, 10, 5, onStepperChange);   
    Component* stepper = createStepperComponentRef(0, 0, stepperState);

    TextState* stepperTextState = createTextStateRef(1, 3, COLOR_ATTENTION, provideStepCounterState);
    Component* stepperText = createTextComponentRef(55, 10, 50, 50, stepperTextState);

    components[0] = &stepCounterComponent;
    components[1] = &resetButton;
    components[2] = stepper;
    components[3] = stepperText;
 
    state = createGroupState(COMPONENTS_COUNT, components);
    group = createGroupComponent(0, 0, &state);
    group.onGesture = onGesture; 

    return &group;
}