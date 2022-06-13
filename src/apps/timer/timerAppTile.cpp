#include <stdio.h>

#include "timerApp.hpp"

static char START[] = "START";
static char STOP[] = "STOP";

static TextState *timerDisplayState;

static const unsigned char COMPONENTS_COUNT = 3;
static void *components[COMPONENTS_COUNT];

static TimerAppApi *api;

static void provideTimerDisplayState(TextState *state)
{
    const unsigned long timerState = (api->getTimerState)();
    snprintf(state->content, sizeof(state->content), "%ul", timerState);
}

static void start(void *c)
{
    (api->startTimer)();
}

static void stop(void *c)
{
    (api->stopTimer)();
}

static void onGesture(Component *component, Gesture gesture)
{
    (api->onGesture)(gesture);
}

static void onButton(Component *component)
{
}

Component *createTimerAppTile(TimerAppApi *timerAppApi, Factory *factory)
{
    api = timerAppApi;

    timerDisplayState = (factory->createTextStateRef)(1, 3, COLOR_ATTENTION, provideTimerDisplayState);

    ButtonComponentState *startButtonState = (factory->createButtonStateRef)(START, EG_ONCE, start);
    ButtonComponentState *stopButtonState = (factory->createButtonStateRef)(STOP, EG_ONCE, stop);

    components[0] = (factory->createButtonComponentRef)(5, 105, 66, 50, startButtonState);
    components[1] = (factory->createButtonComponentRef)(75, 105, 66, 50, stopButtonState);
    components[2] = (factory->createTextComponentRef)(5, 5, 150, 50, timerDisplayState);

    GroupState* state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components); // TODO create factory method for app tiles creation
    Component* group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 
    group->onButton = onButton;

    group->mount(group, 0, 0);
    
    return group;
}