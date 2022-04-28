#include <stdio.h>

#include "clockAppTile.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"
#include "core/watch/rtc.hpp"

static const unsigned char COMPONENTS_COUNT = 3;
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

Component* createClockAppTile(Date *dateRef, int *batteryPercentRef)
{
    date = dateRef;
    batteryPercent = batteryPercentRef;

    hourMinute = createTextState(7, 1, COLOR_INFORMATION, provideHourMinuteState);
    second = createTextState(7, 1, COLOR_INFORMATION, provideSecondState);
    battery = createTextState(1, 2, COLOR_INFORMATION, provideBatteryState);

    hourMinuteComponent = createTextComponent(10, 90, 140, 48, &hourMinute);
    secondComponent = createTextComponent(150, 90, 75, 48, &second);
    batteryDisplayComponent = createTextComponent(135, 150, 50, 50, &battery);

    components[0] = &hourMinuteComponent;
    components[1] = &secondComponent;
    components[2] = &batteryDisplayComponent;
 
    state = createGroupState(COMPONENTS_COUNT, components);
    group = createGroupComponent(0, 0, &state);

    return &group;
}