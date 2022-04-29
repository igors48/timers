#include <stdio.h>

#include <Arduino.h>

#include "clockAppSetTimeTile.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/buttonComponent.hpp"
#include "core/component/group.hpp"
#include "core/watch/rtc.hpp"

static char PLUS[] = "+";
static char SET[] = "SET";

static const unsigned char COMPONENTS_COUNT = 4;
static void* components[COMPONENTS_COUNT];

static TextState hourMinute;
static Component hourMinuteComponent;

static TextState second;
static Component secondComponent;

static ButtonComponentState hourPlusButtonState;
static Component hourPlusButton;

static ButtonComponentState setButtonState;
static Component setButton;

static GroupState state;
static Component group;

static Date *date;

static signed char hourDelta;

static OnGesture onGestureHandler;
static RenderApp renderApp;

static void provideHourMinuteState(TextState *state)
{
    unsigned char hour = date->hour + hourDelta;
    snprintf(state->content, sizeof(state->content), "%02d:%02d", hour, date->minute);
}

static void provideSecondState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), ":%02d", date->second);
}

static void onGesture(Component *component, Gesture gesture)
{
    onGestureHandler(gesture);
}

static void hourPlus()
{
    hourDelta++;
    renderApp(false);
}

static void setTime()
{

}

Component* createClockAppSetTimeTile(Date *dateRef, OnGesture onGestureRef, RenderApp renderAppRef)
{
    date = dateRef;
    onGestureHandler = onGestureRef;
    renderApp = renderAppRef;

    hourDelta = 0;

    hourMinute = createTextState(7, 1, COLOR_INTERACTION, provideHourMinuteState);
    second = createTextState(7, 1, COLOR_INTERACTION, provideSecondState);
    hourPlusButtonState = createButtonState(PLUS, hourPlus);
    setButtonState = createButtonState(SET, setTime);

    hourMinuteComponent = createTextComponent(10, 90, 140, 48, &hourMinute);
    secondComponent = createTextComponent(150, 90, 75, 48, &second);
    hourPlusButton = createButtonComponent(10, 20, 66, 25, &hourPlusButtonState);
    setButton = createButtonComponent(80, 20, 66, 25, &hourPlusButtonState);

    components[0] = &hourMinuteComponent;
    components[1] = &secondComponent;
    components[2] = &hourPlusButton;
    components[3] = &setButton;
 
    state = createGroupState(COMPONENTS_COUNT, components);
    group = createGroupComponent(0, 0, &state);
    group.onGesture = onGesture; 

    return &group;
}