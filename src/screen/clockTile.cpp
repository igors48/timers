#include <stdio.h>

#include "clockTile.hpp"

#include "component/group.hpp"
#include "component/buttonComponent.hpp"
#include "component/textComponent.hpp"

static const unsigned char COMPONENTS_COUNT = 8;
static void* components[COMPONENTS_COUNT];
static GroupState state;

static ButtonComponentState hourMinuteColorChangeButtonState;
static Component hourMinuteColorChangeButton;

static ButtonComponentState secondColorChangeButtonState;
static Component secondColorChangeButton;

static TextState battery;
static Component batteryDisplayComponent;

static TextState stepCounter;
static Component stepCounterComponent;

static TextState date;
static Component dateComponent;

static TextState hourMinute;
static Component hourMinuteComponent;

static TextState second;
static Component secondComponent;

static ButtonComponentState backButtonState;
static Component backButton;

static SetActiveTile setActiveTilePtr;

// todo extract provideXXXX functions to utilities
static void provideBatteryState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "B:%03d%%", watchState->battPercentage);
}

static void provideStepCounterState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "S:%05d", watchState->stepCount);
}

static void provideDateState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "%02d/%02d/%04d", watchState->date.day, watchState->date.month, watchState->date.year);
}

static void provideHourMinuteState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "%02d:%02d", watchState->date.hour, watchState->date.minute);
}

static void provideSecondState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), ":%02d", watchState->date.second);
}

static void changeHourMinuteColor()
{
    if (hourMinute.fontColor == COLOR_INFORMATION)
    {
        hourMinute.fontColor = COLOR_INTERACTION;
    }
    else
    {
        hourMinute.fontColor = COLOR_INFORMATION;
    }
}

static void changeSecondColor()
{
    if (second.fontColor == COLOR_INFORMATION)
    {
        second.fontColor = COLOR_INTERACTION;
    }
    else
    {
        second.fontColor = COLOR_INFORMATION;
    }
}

static void onClick()
{
    setActiveTilePtr(1);
}

static void gestureEventHandler(Component *component, Gesture gesture)
{
    if (gesture == MOVE_LEFT || gesture == MOVE_UP)
    {
        setActiveTilePtr(1);
    }
}

Component createClockTile(SetActiveTile setActiveTile)
{
    setActiveTilePtr = setActiveTile;

    battery = createTextState(1, 2, COLOR_INFORMATION, provideBatteryState);
    stepCounter = createTextState(1, 2, COLOR_INFORMATION, provideStepCounterState);
    date = createTextState(1, 2, COLOR_ATTENTION, provideDateState);
    hourMinute = createTextState(7, 1, COLOR_INFORMATION, provideHourMinuteState);
    second = createTextState(7, 1, COLOR_INFORMATION, provideSecondState);

    hourMinuteColorChangeButtonState = {
        .pressed = false,
        ._pressed = true,
        .handler = changeHourMinuteColor
    };

    secondColorChangeButtonState = {
        .pressed = false,
        ._pressed = true,
        .handler = changeSecondColor
    };

    backButtonState = {
        .pressed = false,
        ._pressed = true,
        .handler = onClick
    };
    
    batteryDisplayComponent = createTextComponent(135, 150, 50, 50, &battery);
    stepCounterComponent = createTextComponent(35, 150, 50, 50, &stepCounter);
    dateComponent = createTextComponent(60, 175, 50, 50, &date);
    hourMinuteComponent = createTextComponent(10, 90, 140, 48, &hourMinute);
    secondComponent = createTextComponent(150, 90, 75, 48, &second);

    hourMinuteColorChangeButton = createButtonComponent(10, 20, 66, 25, &hourMinuteColorChangeButtonState);
    secondColorChangeButton = createButtonComponent(160, 20, 66, 25, &secondColorChangeButtonState);

    backButton = createButtonComponent(60, 195, 66, 45, &backButtonState);

    components[0] = &hourMinuteComponent;
    components[1] = &secondComponent;
    components[2] = &batteryDisplayComponent;
    components[3] = &dateComponent;
    components[4] = &stepCounterComponent;
    components[5] = &hourMinuteColorChangeButton;
    components[6] = &secondColorChangeButton;
    components[7] = &backButton;

    state = createGroupState(COMPONENTS_COUNT, components);
    
    return createTile(&state, gestureEventHandler);
}