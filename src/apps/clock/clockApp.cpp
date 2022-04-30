#include "clockApp.hpp"

#include "core/watch/rtc.hpp"
#include "core/app/tiler.hpp"

static RtcApi *rtcApi;
static PowerApi *powerApi;
static Tiler *tiler;
static SystemApi *systemApi;
static void *backgroundTask;
static Manager *manager;

static Date date;
static int batteryPercent;

static int tileNo;
static Component *clockTile;
static Component *setTimeTile;

static ClockAppApi api;

static void update()
{
    date = (rtcApi->getDate)();
    batteryPercent = (powerApi->getBattPercentage)();
}

static void clockAppActivate(App *app)
{
    update();
    (systemApi->resume)(backgroundTask);
}

static void clockAppDeactivate(App *app)
{
    (systemApi->suspend)(backgroundTask);
}

static Component *clockAppGetActiveTile()
{
    if (tileNo == 0)
    {
        return clockTile;
    }
    else
    {
        return setTimeTile;
    }
}

static void renderApp(bool forced)
{
    tiler->renderApp(forced);
}

static void switchTile()
{
    if (tileNo == 0)
    {
        tileNo = 1;
    }
    else
    {
        tileNo = 0;
    }
    renderApp(true);
}

static void onGesture(Gesture gesture)
{
    bool horizontal = (gesture == MOVE_LEFT) || (gesture == MOVE_RIGHT);
    if (horizontal)
    {
        switchTile();
    }
    else
    {
        (manager->switchApp)(gesture == MOVE_UP);
    }
}

static Date getDate()
{
    return date;
}

static void adjDate(signed char hourDelta)
{
    Date adjustedDate = {
        .year = date.year,
        .month = date.month,
        .day = date.day,
        .hour = (unsigned char)(date.hour + hourDelta),
        .minute = date.minute,
        .second = date.second,
    };
    rtcApi->setDate(adjustedDate);
}

static int getBattery()
{
    return batteryPercent;
}

void clockAppTick()
{
    update();
    renderApp(false);
}

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, Tiler *tilerRef, Manager *managerRef)
{
    rtcApi = rtcApiRef;
    powerApi = powerApiRef;
    tiler = tilerRef;
    manager = managerRef;
    backgroundTask = backgroundTaskHandleRef;
    systemApi = systemApiRef;

    tileNo = 0;

    date = {
        // todo create const
        .year = 0,
        .month = 0,
        .day = 0,
        .hour = 0,
        .minute = 0,
        .second = 0,
    };

    batteryPercent = 0;

    api = {
        .getDate = getDate,
        .adjDate = adjDate,
        .getBattery = getBattery,
        .onGesture = onGesture,
    };

    clockTile = createClockAppTile(&api);
    setTimeTile = createClockAppSetTimeTile(&api);

    return {
        .activate = clockAppActivate,
        .deactivate = clockAppDeactivate,
        .getActiveTile = clockAppGetActiveTile,
    };
}