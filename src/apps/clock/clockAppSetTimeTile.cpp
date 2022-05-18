#include <stdio.h>

#include <LilyGoWatch.h>

#include "clockApp.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"

static char SET[] = "SET";
static char RESET[] = "RESET";

static const unsigned char COMPONENTS_COUNT = 5;
static void* components[COMPONENTS_COUNT];

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
    Serial.println("setTime");
}

static void resetTime(void *context)
{
    Serial.println("resetTime");
}

static void onHourStepperChange(signed short value)
{
    Serial.printf("hour %d\r\n", value);
}

static void onMinuteStepperChange(signed short value)
{
    Serial.printf("minute %d\r\n", value);
}

Component* createClockAppSetTimeTile(ClockAppApi *clockAppApi, Factory *factory)
{
    api = clockAppApi;

    TextState* hourMinute = (factory->createTextStateRef)(7, 1, COLOR_INTERACTION, provideHourMinuteState);
    Component* hourMinuteComponent = (factory->createTextComponentRef)(10, 30, 140, 48, hourMinute);

    ButtonComponentState* setButtonState = (factory->createButtonStateRef)(SET, EG_ONCE, setTime);
    Component* setButton = (factory->createButtonComponentRef)(160, 20, 66, 50, setButtonState);

    ButtonComponentState* resetButtonState = (factory->createButtonStateRef)(RESET, EG_ONCE, resetTime);
    Component* resetButton = (factory->createButtonComponentRef)(160, 170, 66, 50, resetButtonState);

    StepperComponentState* hourStepperState = (factory->createStepperComponentStateRef)(0, 24, 12, onHourStepperChange);
    Component* hourStepper = (factory->createStepperComponentRef)(15, 90, hourStepperState);

    StepperComponentState* minuteStepperState = (factory->createStepperComponentStateRef)(0, 59, 30, onMinuteStepperChange);
    Component* minuteStepper = (factory->createStepperComponentRef)(90, 90, minuteStepperState);

    components[0] = hourMinuteComponent;
    components[1] = setButton;
    components[2] = resetButton;
    components[3] = hourStepper;
    components[4] = minuteStepper;
 
    GroupState* state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components);
    Component* group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 

    group->mount(group, 0, 0);
    
    return group;
}