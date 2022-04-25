#include "clockApp.hpp"

#include "core/watch/rtc.hpp"
#include "core/app/tiler.hpp"

#include "clockAppTile.hpp"

static RtcApi *rtcApi;
static Tiler *tiler;

static Date date;

static Component *tile;

static void clockAppActivate(App *app)
{
    ClockAppState *appState = (ClockAppState *)app->state;
    (app->systemApi->resume)(appState->backgroundTaskHandle);
}

static void clockAppDeactivate(App *app)
{
    ClockAppState *appState = (ClockAppState *)app->state;
    (app->systemApi->suspend)(appState->backgroundTaskHandle);    
}

void clockAppTick()
{
    date = rtcApi->getDate();
    tiler->renderApp(false);
}

App createClockApp(RtcApi *rtcApiRef, Tiler *tilerRef)
{
    rtcApi = rtcApiRef;
    tiler = tilerRef;

    date = { // todo create const
        .year = 0,
        .month = 0,
        .day = 0,
        .hour = 0,
        .minute = 0,
        .second = 0,
    };

    tile = createClockAppTile(&date);

    return {
        .activate = clockAppActivate,
        .deactivate = clockAppDeactivate,
    };
}