#include <stdio.h>

#include "stepApp.hpp"

#include "core/component/buttonComponent.hpp"
#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"
#include "core/component/stepperComponent.hpp"

static char RESET[] = "RESET";

static const unsigned char COMPONENTS_COUNT = 6;
static void *components[COMPONENTS_COUNT];

static TextState* stepCounter;
static Component* stepCounterComponent;

static ButtonComponentState* resetButtonState;
static Component* resetButton;

static GroupState* state;
static Component* group;

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

Component* createStepAppTile(StepAppApi* stepAppApi, Factory *factory)
{
    api = stepAppApi;
    
    stepCounter = (factory->createTextStateRef)(MIDDLE_FONT, 0, COLOR_ATTENTION, provideStepCounterState);
    resetButtonState = (factory->createButtonStateRef)(RESET, EG_ONCE, reset);

    stepCounterComponent = (factory->createTextComponentRef)(55, 120, 50, 50, stepCounter);
    resetButton = (factory->createButtonComponentRef)(60, 195, 66, 25, resetButtonState);

    StepperComponentState* firstStepperState = (factory->createStepperComponentStateRef)(0, 10, 5, onFirstStepperChange);   
    Component* firstStepper = (factory->createStepperComponentRef)(0, 0, firstStepperState);

    StepperComponentState* secondStepperState = (factory->createStepperComponentStateRef)(0, 10, 5, onSecondStepperChange);   
    Component* secondStepper = (factory->createStepperComponentRef)(190, 0, secondStepperState);

    TextState* firstStepperTextState = (factory->createTextStateRef)(1, 3, COLOR_ATTENTION, provideFirstStepperState);
    Component* firstStepperText = (factory->createTextComponentRef)(55, 10, 50, 50, firstStepperTextState);

    TextState* secondStepperTextState = (factory->createTextStateRef)(1, 3, COLOR_ATTENTION, provideSecondStepperState);
    Component* secondStepperText = (factory->createTextComponentRef)(120, 10, 50, 50, secondStepperTextState);

    components[0] = stepCounterComponent;
    components[1] = resetButton;
    components[2] = firstStepperText;
    components[3] = secondStepperText;
    components[4] = secondStepper;
    components[5] = firstStepper;
 
    state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components);
    group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 

    group->mount(group, 0, 0);
    
    return group;
}