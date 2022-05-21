#include <stdio.h>

#include "clockApp.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"

static char TITLE[] = "SET TIME";
static char SET[] = "SET";
static char RESET[] = "RESET";

static const unsigned char COMPONENTS_COUNT = 6;
static void* components[COMPONENTS_COUNT];

static unsigned char hour;
static unsigned char minute;

static ClockAppApi *api;

static StepperComponentState* hourStepperState;
static StepperComponentState* minuteStepperState;

static void provideTitle(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "%s", TITLE);
}

static void provideHourMinuteState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "%02d:%02d", hour, minute);
}

static void onGesture(Component *component, Gesture gesture)
{
    api->onGesture(gesture);
}

static void setTime(void *context)
{
    api->setTime(hour, minute);
}

static void resetTime(void *context)
{
    Date date = api->getDate();
    hour = date.hour;
    minute = date.minute;
    (hourStepperState->setValue)(hourStepperState, date.hour);
    (minuteStepperState->setValue)(minuteStepperState, date.minute);
}

static void onHourStepperChange(signed short value)
{
    hour = value;
}

static void onMinuteStepperChange(signed short value)
{
    minute = value;
}

Component* createClockAppSetTimeTile(ClockAppApi *clockAppApi, Factory *factory)
{
    api = clockAppApi;

    hour = 12;
    minute = 30;

    TextState* title = (factory->createTextStateRef)(2, 1, COLOR_INFORMATION, provideTitle);
    TextState* hourMinute = (factory->createTextStateRef)(7, 1, COLOR_INTERACTION, provideHourMinuteState);
    ButtonComponentState* setButtonState = (factory->createButtonStateRef)(SET, EG_ONCE, setTime);
    ButtonComponentState* resetButtonState = (factory->createButtonStateRef)(RESET, EG_ONCE, resetTime);
    hourStepperState = (factory->createStepperComponentStateRef)(0, 23, hour, onHourStepperChange);
    minuteStepperState = (factory->createStepperComponentStateRef)(0, 59, minute, onMinuteStepperChange);

    components[0] = (factory->createTextComponentRef)(10, 40, 140, 48, hourMinute);
    components[1] = (factory->createButtonComponentRef)(160, 20, 66, 50, setButtonState);
    components[2] = (factory->createButtonComponentRef)(160, 170, 66, 50, resetButtonState);
    components[3] = (factory->createStepperComponentRef)(15, 100, hourStepperState);
    components[4] = (factory->createStepperComponentRef)(90, 100, minuteStepperState);
    components[5] = (factory->createTextComponentRef)(5, 5, 140, 48, title);
 
    GroupState* state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components);
    Component* group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 

    group->mount(group, 0, 0);
    
    return group;
}