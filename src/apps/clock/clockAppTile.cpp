#include <stdio.h>

#include <Arduino.h>

#include "clockApp.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"

static const unsigned char COMPONENTS_COUNT = 8;
static void* components[COMPONENTS_COUNT];

static char WAKE_UP_REASON[] = "WUR: %d";
static char TIME_TO_SLEEP[] = "TTS: %d";
static char NEXT_WAKE_UP[] = "NWU: %d";

static TextState* hourMinute;
static TextState* second;
static TextState* battery;
static TextState* dateState;
static TextState* stepCounter;
static TextState* wakeUpReason;
static TextState* timeToSleep;
static TextState* nextWakeUp;

static GroupState* state;
static Component* group;

static ClockAppApi *api;

static void provideWakeUpReason(TextState *state)
{
    WakeUpReason wakeUpReason = (api->getWakeUpReason)();
    snprintf(state->content, sizeof(state->content), WAKE_UP_REASON, wakeUpReason);
}

static void provideTimeToSleep(TextState *state)
{
    unsigned short timeToSleep = (api->getTimeToSleep)();
    snprintf(state->content, sizeof(state->content), TIME_TO_SLEEP, timeToSleep);
}

static void provideNextWakeUp(TextState *state)
{
    unsigned int nextWakeUpPeriod = (api->getNextWakeUpPeriod)();
    snprintf(state->content, sizeof(state->content), NEXT_WAKE_UP, nextWakeUpPeriod);
}

static void provideHourMinuteState(TextState *state)
{
    Date date = api->getDate();
    snprintf(state->content, sizeof(state->content), "%02d:%02d", date.hour, date.minute);
}

static void provideSecondState(TextState *state)
{
    Date date = api->getDate();
    snprintf(state->content, sizeof(state->content), ":%02d", date.second);
}

static void provideBatteryState(TextState *state)
{
    int percent = api->getBattery();
    snprintf(state->content, sizeof(state->content), "B:%03d%%", percent);
}

static void provideDateState(TextState *state)
{
    Date date = api->getDate();
    snprintf(state->content, sizeof(state->content), "%02d/%02d/%04d", date.day, date.month, date.year);
}

static void provideStepCounterState(TextState *state)
{
    unsigned int stepCount = api->getStepCounter();
    snprintf(state->content, sizeof(state->content), "S:%05d", stepCount);
}

static void onGesture(Component *component, Gesture gesture)
{
    api->onGesture(gesture);
}

Component* createClockAppTile(ClockAppApi *clockAppApi, Factory *factory)
{
    api = clockAppApi;

    hourMinute = (factory->createTextStateRef)(7, 1, COLOR_INFORMATION, provideHourMinuteState);
    second = (factory->createTextStateRef)(7, 1, COLOR_INFORMATION, provideSecondState);
    battery = (factory->createTextStateRef)(1, 2, COLOR_INFORMATION, provideBatteryState);
    dateState = (factory->createTextStateRef)(1, 2, COLOR_ATTENTION, provideDateState);
    stepCounter = (factory->createTextStateRef)(1, 2, COLOR_INFORMATION, provideStepCounterState);
    wakeUpReason = (factory->createTextStateRef)(1, 2, COLOR_INTERACTION, provideWakeUpReason);
    timeToSleep = (factory->createTextStateRef)(1, 2, COLOR_INTERACTION, provideTimeToSleep);
    nextWakeUp = (factory->createTextStateRef)(1, 2, COLOR_INTERACTION, provideNextWakeUp);

    components[0] = (factory->createTextComponentRef)(10, 30, 140, 48, hourMinute);
    components[1] = (factory->createTextComponentRef)(150, 30, 75, 48, second);
    components[2] = (factory->createTextComponentRef)(135, 90, 50, 50, battery);
    components[3] = (factory->createTextComponentRef)(60, 115, 50, 50, dateState);
    components[4] = (factory->createTextComponentRef)(35, 90, 50, 50, stepCounter);
    components[5] = (factory->createTextComponentRef)(10, 180, 50, 50, wakeUpReason);
    components[6] = (factory->createTextComponentRef)(10, 200, 50, 50, timeToSleep);
    components[7] = (factory->createTextComponentRef)(10, 220, 50, 50, nextWakeUp);
 
    state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components);
    group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 

    group->mount(group, 0, 0);

    return group;
}