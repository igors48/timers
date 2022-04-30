#include <stdio.h>

#include "stepApp.hpp"

#include "core/component/buttonComponent.hpp"
#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"

static char RESET[] = "RESET";

static const unsigned char COMPONENTS_COUNT = 2;
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

static void reset()
{
    (api->resetStepCounter)();
    (api->render)(false);
}

static void onGesture(Component *component, Gesture gesture)
{
    (api->onGesture)(gesture);
}

Component *createStepAppTile(StepAppApi *stepAppApi)
{
    stepCounter = createTextState(1, 3, COLOR_ATTENTION, provideStepCounterState);
    resetButtonState = createButtonState(RESET, reset);

    stepCounterComponent = createTextComponent(55, 120, 50, 50, &stepCounter);
    resetButton = createButtonComponent(60, 195, 66, 25, &resetButtonState);
    
    components[0] = &stepCounterComponent;
    components[1] = &resetButton;
 
    state = createGroupState(COMPONENTS_COUNT, components);
    group = createGroupComponent(0, 0, &state);
    group.onGesture = onGesture; 

    return &group;
}