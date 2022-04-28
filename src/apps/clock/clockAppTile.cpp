#include <stdio.h>

#include "clockAppTile.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"
#include "core/watch/rtc.hpp"

#include <Arduino.h>

static const unsigned char COMPONENTS_COUNT = 4;
static void* components[COMPONENTS_COUNT];

static TextState hourMinute;
static Component hourMinuteComponent;

static TextState second;
static Component secondComponent;

static TextState battery;
static Component batteryDisplayComponent;

static GroupState state;
static Component group;

static Date *date;
static int *batteryPercent;

static TextState dateState;
static Component dateComponent;

static void provideHourMinuteState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "%02d:%02d", date->hour, date->minute);
}

static void provideSecondState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), ":%02d", date->second);
}

static void provideBatteryState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "B:%03d%%", *batteryPercent);
}

static void provideDateState(TextState *state)
{
    snprintf(state->content, sizeof(state->content), "%02d/%02d/%04d", date->day, date->month, date->year);
}

static void onGesture(Component *component, Gesture gesture)
{
    Serial.println("gesture");
}

Component* createClockAppTile(Date *dateRef, int *batteryPercentRef)
{
    date = dateRef;
    batteryPercent = batteryPercentRef;

    hourMinute = createTextState(7, 1, COLOR_INFORMATION, provideHourMinuteState);
    second = createTextState(7, 1, COLOR_INFORMATION, provideSecondState);
    battery = createTextState(1, 2, COLOR_INFORMATION, provideBatteryState);
    dateState = createTextState(1, 2, COLOR_ATTENTION, provideDateState);

    hourMinuteComponent = createTextComponent(10, 90, 140, 48, &hourMinute);
    secondComponent = createTextComponent(150, 90, 75, 48, &second);
    batteryDisplayComponent = createTextComponent(135, 150, 50, 50, &battery);
    dateComponent = createTextComponent(60, 175, 50, 50, &dateState);

    components[0] = &hourMinuteComponent;
    components[1] = &secondComponent;
    components[2] = &batteryDisplayComponent;
    components[3] = &dateComponent;
 
    state = createGroupState(COMPONENTS_COUNT, components);
    group = createGroupComponent(0, 0, &state);
    group.onGesture = onGesture; 

    return &group;
}