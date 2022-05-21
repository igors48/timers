#include <stdio.h>

#include "clockApp.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"

static char TITLE[] = "SET DATE";
static char SET[] = "SET";
static char RESET[] = "RESET";

static const unsigned char COMPONENTS_COUNT = 6;
static void* components[COMPONENTS_COUNT];

static unsigned char day;
static unsigned char month;

static ClockAppApi *api;

static StepperComponentState* dayStepperState;
static StepperComponentState* monthStepperState;

static void provideTitle(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "%s", TITLE);
}

static void provideDayMonthState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "%02d %02d", day, month);
}

static void onGesture(Component *component, Gesture gesture)
{
    api->onGesture(gesture);
}

static void setDate(void *context)
{
    api->setDate(day, month);
}

static void resetDate(void *context)
{
    Date date = api->getDate();
    day = date.day;
    month = date.month;
    (dayStepperState->setValue)(dayStepperState, day);
    (monthStepperState->setValue)(monthStepperState, month);
}

static void onDayStepperChange(signed short value)
{
    day = value;
}

static void onMonthStepperChange(signed short value)
{
    month = value;
}

Component* createClockAppSetDateTile(ClockAppApi *clockAppApi, Factory *factory)
{
    api = clockAppApi;

    day = 15;
    month = 6;

    TextState* title = (factory->createTextStateRef)(2, 1, COLOR_INFORMATION, provideTitle);
    TextState* dayMonth = (factory->createTextStateRef)(7, 1, COLOR_INTERACTION, provideDayMonthState);
    ButtonComponentState* setButtonState = (factory->createButtonStateRef)(SET, EG_ONCE, setDate);
    ButtonComponentState* resetButtonState = (factory->createButtonStateRef)(RESET, EG_ONCE, resetDate);
    dayStepperState = (factory->createStepperComponentStateRef)(1, 31, day, onDayStepperChange);
    monthStepperState = (factory->createStepperComponentStateRef)(5, 12, month, onMonthStepperChange);

    components[0] = (factory->createTextComponentRef)(10, 40, 140, 48, dayMonth);
    components[1] = (factory->createButtonComponentRef)(160, 20, 66, 50, setButtonState);
    components[2] = (factory->createButtonComponentRef)(160, 170, 66, 50, resetButtonState);
    components[3] = (factory->createStepperComponentRef)(15, 100, dayStepperState);
    components[4] = (factory->createStepperComponentRef)(90, 100, monthStepperState);
    components[5] = (factory->createTextComponentRef)(5, 5, 140, 48, title);
 
    GroupState* state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components);
    Component* group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 

    group->mount(group, 0, 0);
    
    return group;
}