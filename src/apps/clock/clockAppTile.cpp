#include <stdio.h>

#include "clockApp.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"

#include "lineCalendar.hpp"

static const unsigned char COMPONENTS_COUNT = 9;
static void *components[COMPONENTS_COUNT];

static char WAKE_UP_REASON[] = "WUR: %d";
static char TIME_TO_SLEEP[] = "TTS: %ds";
static char NEXT_WAKE_UP[] = "NWU: %dm %ds";

static TextState *hourMinute;
static TextState *second;
static TextState *battery;
static TextState *dateState;
static TextState *stepCounter;
static TextState *wakeUpReason;
static TextState *timeToSleep;
static TextState *nextWakeUp;

static LineCalendarContext lineCalendarContext;
static Component lineCalendar;

static GroupState *state;
static Component *group;

static ClockAppApi *api;

unsigned char secondValue;
unsigned long startEffectTick = 0;

static void provideContext(LineCalendarContext *state)
{
    const Date date = api->getDate();
    state->dayOfWeek = date.dayOfWeek;
}

static void provideWakeUpReason(TextState *state)
{
    const WakeUpReason wakeUpReason = (api->getWakeUpReason)();
    snprintf(state->content, sizeof(state->content), WAKE_UP_REASON, wakeUpReason);
}

static void provideTimeToSleep(TextState *state)
{
    const unsigned long timeToSleep = (api->getTimeToSleep)();
    snprintf(state->content, sizeof(state->content), TIME_TO_SLEEP, timeToSleep);
}

static void provideNextWakeUp(TextState *state)
{
    const unsigned int nextWakeUpPeriod = (api->getNextWakeUpPeriod)() / 1000;
    const unsigned int minutes = nextWakeUpPeriod / 60;
    const unsigned int seconds = nextWakeUpPeriod - minutes * 60;
    snprintf(state->content, sizeof(state->content), NEXT_WAKE_UP, minutes, seconds);
}

static void provideHourMinuteState(TextState *state)
{
    const Date date = api->getDate();
    snprintf(state->content, sizeof(state->content), "%02d:%02d", date.hour, date.minute);
}

static void provideSecondState(TextState *state)
{
    const Date date = api->getDate();
    if (date.second != secondValue)
    {
        secondValue = date.second;
        startEffectTick = (api->getTickCount)();
    }
    snprintf(state->content, sizeof(state->content), ":%02d", date.second);
}

static void provideBatteryState(TextState *state)
{
    const int percent = api->getBattery();
    snprintf(state->content, sizeof(state->content), "B:%03d%%", percent);
}

static void provideDateState(TextState *state)
{
    const Date date = api->getDate();
    snprintf(state->content, sizeof(state->content), "%02d/%02d/%04d", date.day, date.month, date.year);
}

static void provideStepCounterState(TextState *state)
{
    const unsigned int stepCount = api->getStepCounter();
    snprintf(state->content, sizeof(state->content), "S:%05d", stepCount);
}

static void onGesture(Component *component, Gesture gesture)
{
    api->onGesture(gesture);
}

static void renderWithInactiveSegments(Component *component, bool forced, TftApi *tftApi, const char inactiveText[])
{
    TextState *state = (TextState *)component->state;
    (tftApi->setTextSize)(state->size);
    (tftApi->setTextFont)(state->font);
    (tftApi->setTextColor)(0x10A2, state->backColor); // 20, 20, 20
    (tftApi->drawString)(inactiveText, component->x, component->y);
    (tftApi->setTextColorOnly)(state->fontColor);
    (tftApi->drawString)(state->content, component->x, component->y);
}

static void secondComponentRender(Component *component, bool forced, TftApi *tftApi)
{
    renderWithInactiveSegments(component, forced, tftApi, ":88");
}

static void hourMinuteComponentRender(Component *component, bool forced, TftApi *tftApi)
{
    renderWithInactiveSegments(component, forced, tftApi, "88:88");
}

static void fadeAwayEffect(unsigned long start, unsigned long current, unsigned long duration, TextState* state)
{
    if (start == 0)
    {
        return;
    }
    const unsigned long passed = current - start;
    if (passed > duration)
    {
        return;
    }
    const unsigned long rest = duration - passed;
    const unsigned char colorRange = 255 - 20;
    const float ratio = ((float)colorRange) / ((float)duration);
    const unsigned char color = 20 + rest * ratio;
    const unsigned short color565 = ((color & 0xF8) << 8) | ((color & 0xFC) << 3) | (color >> 3);
    second->fontColor = color565;
}

static void onTick(Component *component, unsigned long tick)
{
    fadeAwayEffect(startEffectTick, tick, 900, second);
}

Component *createClockAppTile(ClockAppApi *clockAppApi, Factory *factory)
{
    api = clockAppApi;

    secondValue = 255;

    hourMinute = (factory->createTextStateRef)(7, 1, COLOR_INFORMATION, provideHourMinuteState);
    second = (factory->createTextStateRef)(7, 1, COLOR_INFORMATION, provideSecondState);
    battery = (factory->createTextStateRef)(MEDIUM_FONT, 0, COLOR_INFORMATION, provideBatteryState);
    dateState = (factory->createTextStateRef)(MEDIUM_FONT, 0, COLOR_ATTENTION, provideDateState);
    stepCounter = (factory->createTextStateRef)(MEDIUM_FONT, 0, COLOR_INFORMATION, provideStepCounterState);
    wakeUpReason = (factory->createTextStateRef)(SMALL_FONT, 0, COLOR_INTERACTION, provideWakeUpReason);
    timeToSleep = (factory->createTextStateRef)(SMALL_FONT, 0, COLOR_INTERACTION, provideTimeToSleep);
    nextWakeUp = (factory->createTextStateRef)(SMALL_FONT, 0, COLOR_INTERACTION, provideNextWakeUp);
    
    lineCalendarContext = createLineCalendarContext(provideContext);
    lineCalendar = createLineCalendarComponent(0, 115, &lineCalendarContext);

    Component *hourMinuteText = (factory->createTextComponentRef)(12, 50, 140, 48, hourMinute);
    hourMinuteText->render = hourMinuteComponentRender;

    Component *secondText = (factory->createTextComponentRef)(152, 50, 75, 48, second);
    secondText->render = secondComponentRender;

    components[0] = hourMinuteText;
    components[1] = secondText;
    components[2] = (factory->createTextComponentRef)(165, 5, 50, 50, battery);
    components[3] = (factory->createTextComponentRef)(60, 150, 50, 50, dateState);
    components[4] = (factory->createTextComponentRef)(5, 5, 50, 50, stepCounter);
    components[5] = (factory->createTextComponentRef)(10, 180, 50, 50, wakeUpReason);
    components[6] = (factory->createTextComponentRef)(10, 200, 50, 50, timeToSleep);
    components[7] = (factory->createTextComponentRef)(10, 220, 50, 50, nextWakeUp);
    components[8] = &lineCalendar;

    state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components);

    group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture;
    group->onTick = onTick;

    group->mount(group, 0, 0);

    return group;
}