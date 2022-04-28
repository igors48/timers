#include "clockApp.hpp"

#include "core/watch/rtc.hpp"
#include "core/app/tiler.hpp"

#include "clockAppTile.hpp"

static RtcApi *rtcApi;
static PowerApi *powerApi;
static Tiler *tiler;

static Date date;
static int batteryPercent;

static Component *tile;

static ClockAppState state;

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

static Component* clockAppGetActiveTile()
{
    return tile;
}

void clockAppTick()
{
    date = (rtcApi->getDate)();
    batteryPercent = (powerApi->getBattPercentage)();
    tiler->renderApp(false);
}

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, Tiler *tilerRef)
{
    rtcApi = rtcApiRef;
    powerApi = powerApiRef;
    tiler = tilerRef;

    date = { // todo create const
        .year = 0,
        .month = 0,
        .day = 0,
        .hour = 0,
        .minute = 0,
        .second = 0,
    };

    batteryPercent = 0;

    tile = createClockAppTile(&date, &batteryPercent);

    state = {
        .backgroundTaskHandle = backgroundTaskHandleRef,
    };

    return {
        .activate = clockAppActivate,
        .deactivate = clockAppDeactivate,
        .getActiveTile = clockAppGetActiveTile,
        .systemApi = systemApiRef,
        .state = &state,
    };
}