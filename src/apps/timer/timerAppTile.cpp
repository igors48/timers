#include "timerApp.hpp"

static char START[] = "START";

static const unsigned char COMPONENTS_COUNT = 1;
static void *components[COMPONENTS_COUNT];

static TimerAppApi *api;

static void start(void *c)
{
}

static void onGesture(Component *component, Gesture gesture)
{
}

static void onButton(Component *component)
{
}

Component *createTimerAppTile(TimerAppApi *timerAppApi, Factory *factory)
{
    api = timerAppApi;

    ButtonComponentState *beepButtonState = (factory->createButtonStateRef)(START, EG_ONCE, start);

    components[0] = (factory->createButtonComponentRef)(160, 20, 66, 50, beepButtonState);

    GroupState* state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components); // TODO create factory method for app tiles creation
    Component* group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 
    group->onButton = onButton;

    group->mount(group, 0, 0);
    
    return group;
}