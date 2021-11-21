#include <stdio.h>

#include "clockTile.hpp"

#include "component/group.hpp"
#include "component/buttonComponent.hpp"
#include "component/textComponent.hpp"

const unsigned char CLOCK_TILE_COMPONENTS_COUNT = 8;
void* clockTileComponents[CLOCK_TILE_COMPONENTS_COUNT];
GroupState clockTileState;

ButtonComponentState hourMinuteColorChangeButtonState;
Component hourMinuteColorChangeButton;

ButtonComponentState secondColorChangeButtonState;
Component secondColorChangeButton;

TextState battery;
Component batteryDisplayComponent;

static TextState stepCounter;
static Component stepCounterComponent;

TextState date;
Component dateComponent;

static TextState hourMinute;
static Component hourMinuteComponent;

static TextState second;
static Component secondComponent;

static ButtonComponentState backButtonState;
static Component backButton;

static SetActiveTile setActiveTilePtr;

// todo extract provideXXXX functions to utilities
void provideBatteryState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "B:%03d%%", watchState->battPercentage);
}

static void provideStepCounterState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "S:%05d", watchState->stepCount);
}

void provideDateState(TextState *state, WatchState *watchState)
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

void changeHourMinuteColor()
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

void changeSecondColor()
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

Component* mount(Component *component)
{
    (component->mount)(component, 0, 0);
    return component;
}

static void onClick()
{
    setActiveTilePtr(1);
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

    clockTileComponents[0] = mount(&hourMinuteComponent);
    clockTileComponents[1] = mount(&secondComponent);
    clockTileComponents[2] = mount(&batteryDisplayComponent);
    clockTileComponents[3] = mount(&dateComponent);
    clockTileComponents[4] = mount(&stepCounterComponent);
    clockTileComponents[5] = mount(&hourMinuteColorChangeButton);
    clockTileComponents[6] = mount(&secondColorChangeButton);
    clockTileComponents[7] = mount(&backButton);

    clockTileState = createGroupState(CLOCK_TILE_COMPONENTS_COUNT, clockTileComponents);

    return createGroupComponent(0, 0, &clockTileState);
}