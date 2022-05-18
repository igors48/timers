#include <stdio.h>

#include "clockApp.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"

static char SET[] = "SET";

static const unsigned char COMPONENTS_COUNT = 2;
static void* components[COMPONENTS_COUNT];

static TextState* hourMinute;
static Component* hourMinuteComponent;

static ButtonComponentState* setButtonState;
static Component* setButton;

static GroupState* state;
static Component* group;

static ClockAppApi *api;

static void provideHourMinuteState(TextState *state)
{
    Date date = api->getDate();
    unsigned char hour = date.hour;
    snprintf(state->content, sizeof(state->content), "%02d:%02d", hour, date.minute);
}

static void onGesture(Component *component, Gesture gesture)
{
    api->onGesture(gesture);
}


static void setTime(void *context)
{
}

Component* createClockAppSetTimeTile(ClockAppApi *clockAppApi, Factory *factory)
{
    api = clockAppApi;

    hourMinute = (factory->createTextStateRef)(7, 1, COLOR_INTERACTION, provideHourMinuteState);
    setButtonState = (factory->createButtonStateRef)(SET, EG_ONCE, setTime);

    hourMinuteComponent = (factory->createTextComponentRef)(10, 90, 140, 48, hourMinute);
    setButton = (factory->createButtonComponentRef)(80, 20, 66, 50, setButtonState);

    components[0] = hourMinuteComponent;
    components[1] = setButton;
 
    state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components);
    group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 

    group->mount(group, 0, 0);
    
    return group;
}