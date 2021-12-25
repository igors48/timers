#include <stdio.h>

#include "stepCounterTile.hpp"

#include "component/buttonComponent.hpp"
#include "component/group.hpp"
#include "component/textComponent.hpp"

static const unsigned char COMPONENTS_COUNT = 4;
static void* components[COMPONENTS_COUNT];
static GroupState state;

static TextState hourMinute;
static Component hourMinuteComponent;

static TextState second;
static Component secondComponent;

static TextState stepCounter;
static Component stepCounterComponent;

static ButtonComponentState backButtonState;
static Component backButton;

static SetActiveTile setActiveTilePtr;
static SoundApi *soundApiPtr;

static void provideHourMinuteState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "%02d:%02d", watchState->date.hour, watchState->date.minute);
}

static void provideSecondState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), ":%02d", watchState->date.second);
}

static void provideStepCounterState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "S:%05d", watchState->stepCount);
}

static void onClick()
{
    soundApiPtr->beep();
    //setActiveTilePtr(0);
}

static void gestureEventHandler(Component *component, Gesture gesture)
{
    if (gesture == MOVE_RIGHT || gesture == MOVE_DOWN)
    {
        setActiveTilePtr(0);
    }
}

Component createStepCounterTile(SetActiveTile setActiveTile, SoundApi *soundApi)
{
    setActiveTilePtr = setActiveTile;
    soundApiPtr = soundApi;

    hourMinute = createTextState(7, 1, COLOR_INFORMATION, provideHourMinuteState);
    second = createTextState(7, 1, COLOR_INFORMATION, provideSecondState);
    stepCounter = createTextState(1, 3, COLOR_ATTENTION, provideStepCounterState);

    backButtonState = {
        .pressed = false,
        ._pressed = true,
        .handler = onClick
    };

    hourMinuteComponent = createTextComponent(10, 60, 140, 48, &hourMinute);
    secondComponent = createTextComponent(150, 60, 75, 48, &second);
    stepCounterComponent = createTextComponent(55, 120, 50, 50, &stepCounter);
    backButton = createButtonComponent(60, 195, 66, 25, &backButtonState);

    components[0] = &hourMinuteComponent;
    components[1] = &secondComponent;
    components[2] = &stepCounterComponent;
    components[3] = &backButton;

    state = createGroupState(COMPONENTS_COUNT, components);

    return createTile(&state, gestureEventHandler);
}