#include <Arduino.h>

#include "screen.hpp"

#include "component/buttonComponent.hpp"
#include "component/textComponent.hpp"

ButtonComponentState hourMinuteColorChangeButtonState;
Component hourMinuteColorChangeButton;

ButtonComponentState secondColorChangeButtonState;
Component secondColorChangeButton;

TextState battery;
Component batteryDisplayComponent;

TextState stepCounter;
Component stepCounterComponent;

TextState date;
Component dateComponent;

TextState hourMinute;
Component hourMinuteComponent;

TextState second;
Component secondComponent;

void provideBatteryState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "B:%03d%%", watchState->battPercentage);
}

void provideStepCounterState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "S:%05d", watchState->stepCount);
}

void provideDateState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "%02d/%02d/%04d", watchState->date.day, watchState->date.month, watchState->date.year);
}

void provideHourMinuteState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), "%02d:%02d", watchState->date.hour, watchState->date.minute);
}

void provideSecondState(TextState *state, WatchState *watchState)
{
    snprintf(state->content, sizeof(state->content), ":%02d", watchState->date.second);
}

void changeHourMinuteColor()
{
    //hourMinuteComponentColorChange(&hourMinuteComponentState);
}

void changeSecondColor()
{
    //secondComponentColorChange(&secondComponentState);
}

Component* mount(Component *component)
{
    (component->mount)(component, 0, 0);
    return component;
}

void createComponents(void* components[])
{
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

    batteryDisplayComponent = createTextComponent(135, 150, 50, 50, &battery);
    stepCounterComponent = createTextComponent(35, 150, 50, 50, &stepCounter);
    dateComponent = createTextComponent(60, 175, 50, 50, &date);
    hourMinuteComponent = createTextComponent(10, 90, 140, 48, &hourMinute);
    secondComponent = createTextComponent(150, 90, 75, 48, &second);

    hourMinuteColorChangeButton = createButtonComponent(10, 20, 66, 25, &hourMinuteColorChangeButtonState);
    secondColorChangeButton = createButtonComponent(160, 20, 66, 25, &secondColorChangeButtonState);

    components[0] = mount(&hourMinuteComponent);
    components[1] = mount(&secondComponent);
    components[2] = mount(&batteryDisplayComponent);
    components[3] = mount(&dateComponent);
    components[4] = mount(&stepCounterComponent);
    components[5] = mount(&hourMinuteColorChangeButton);
    components[6] = mount(&secondColorChangeButton);
}