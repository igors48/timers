#include "clockApp.hpp"

#include "core/watch/rtc.hpp"
#include "core/app/tiler.hpp"

static RtcApi *rtcApi;
static Tiler *tiler;

static unsigned char hour;
static unsigned char minute;
static unsigned char second;

static void clockAppActivate(App *app)
{
    ClockAppState *appState = (ClockAppState *)app->state;
    (app->systemApi->resume)(appState->backgroundTaskHandle);
}

static void clockAppDeactivate(App *app)
{
    ClockAppState *appState = (ClockAppState *)app->state;
    (app->systemApi->resume)(appState->backgroundTaskHandle);    
}

void clockAppTick()
{
    Date date = rtcApi->getDate();
    hour = date.hour;
    minute = date.minute;
    second = date.second;

    tiler->renderApp(false);
}

App createClockApp(RtcApi *rtcApiRef, Tiler *tilerRef)
{
    rtcApi = rtcApiRef;
    tiler = tilerRef;

    hour = 0;
    minute = 0;
    second = 0;

    return {
        .activate = clockAppActivate,
        .deactivate = clockAppDeactivate,
    };
}