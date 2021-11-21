#include <stdio.h>

#include "stepCounterTile.hpp"

#include "component/buttonComponent.hpp"
#include "component/group.hpp"
#include "component/textComponent.hpp"

const unsigned char STEP_COUNTER_TILE_COMPONENTS_COUNT = 4;
void* stepCounterTileComponents[STEP_COUNTER_TILE_COMPONENTS_COUNT];
GroupState stepCounterTileState;

static TextState hourMinute;
static Component hourMinuteComponent;

static TextState second;
static Component secondComponent;

static TextState stepCounter;
static Component stepCounterComponent;

static ButtonComponentState backButtonState;
static Component backButton;

static SetActiveTile setActiveTilePtr;

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
    setActiveTilePtr(0);
}

Component createStepCounterTile(SetActiveTile setActiveTile)
{
    setActiveTilePtr = setActiveTile;

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

    stepCounterTileComponents[0] = &hourMinuteComponent;
    stepCounterTileComponents[1] = &secondComponent;
    stepCounterTileComponents[2] = &stepCounterComponent;
    stepCounterTileComponents[3] = &backButton;

    stepCounterTileState = createGroupState(STEP_COUNTER_TILE_COMPONENTS_COUNT, stepCounterTileComponents);

    return createGroupComponent(0, 0, &stepCounterTileState);
}