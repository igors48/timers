#include <stdio.h>

#include "clockApp.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/buttonComponent.hpp"
#include "core/component/group.hpp"

static char PLUS[] = "+";
static char MINUS[] = "-";
static char SET[] = "SET";

static const unsigned char COMPONENTS_COUNT = 5;
static void* components[COMPONENTS_COUNT];

static TextState hourMinute;
static Component hourMinuteComponent;

static TextState second;
static Component secondComponent;

static ButtonComponentState hourPlusButtonState;
static Component hourPlusButton;

static ButtonComponentState hourMinusButtonState;
static Component hourMinusButton;

static ButtonComponentState setButtonState;
static Component setButton;

static GroupState state;
static Component group;

static signed char hourDelta;

static ClockAppApi *api;

static void provideHourMinuteState(TextState *state)
{
    Date date = api->getDate();
    unsigned char hour = date.hour + hourDelta;
    snprintf(state->content, sizeof(state->content), "%02d:%02d", hour, date.minute);
}

static void provideSecondState(TextState *state)
{
    Date date = api->getDate();
    snprintf(state->content, sizeof(state->content), ":%02d", date.second);
}

static void onGesture(Component *component, Gesture gesture)
{
    api->onGesture(gesture);
}

static void hourPlus()
{
    hourDelta++;
}

static void hourMinus()
{
    hourDelta--;
}

static void setTime()
{
    api->adjDate(hourDelta);
    hourDelta = 0;
}

Component* createClockAppSetTimeTile(ClockAppApi *clockAppApi)
{
    api = clockAppApi;

    hourDelta = 0;

    hourMinute = createTextState(7, 1, COLOR_INTERACTION, provideHourMinuteState);
    second = createTextState(7, 1, COLOR_INTERACTION, provideSecondState);
    hourPlusButtonState = createButtonState(PLUS, hourPlus);
    hourMinusButtonState = createButtonState(MINUS, hourMinus);
    setButtonState = createButtonState(SET, setTime);

    hourMinuteComponent = createTextComponent(10, 90, 140, 48, &hourMinute);
    secondComponent = createTextComponent(150, 90, 75, 48, &second);
    hourPlusButton = createButtonComponent(10, 20, 66, 25, &hourPlusButtonState);
    hourMinusButton = createButtonComponent(10, 180, 66, 25, &hourMinusButtonState);
    setButton = createButtonComponent(80, 20, 66, 25, &setButtonState);

    components[0] = &hourMinuteComponent;
    components[1] = &secondComponent;
    components[2] = &hourPlusButton;
    components[3] = &hourMinusButton;
    components[4] = &setButton;
 
    state = createGroupState(COMPONENTS_COUNT, components);
    group = createGroupComponent(0, 0, &state);
    group.onGesture = onGesture; 

    return &group;
}