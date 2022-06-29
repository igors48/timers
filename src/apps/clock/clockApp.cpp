#include "clockApp.hpp"

#include "core/watch/rtc.hpp"
#include "core/app/tiler.hpp"

static RtcApi *rtcApi;
static PowerApi *powerApi;
static BmaApi *bmaApi;
static Tiler *tiler;
static SystemApi *systemApi;
static void *backgroundTask;
static Manager *manager;
static SupervisorApi *supervisorApi;

static Date date;
static int batteryPercent;
static unsigned int stepCounter;

static int tileNo;
static Component *clockTile;
static Component *setTimeTile;
static Component *setDateTile;

static ClockAppApi api;

static void update()
{
    date = (rtcApi->getDate)();
    batteryPercent = (powerApi->getBattPercentage)();
    stepCounter = (bmaApi->getCounter)();
}

static void clockAppActivate()
{
    update();
    (systemApi->resume)(backgroundTask);
}

static void clockAppDeactivate()
{
    (systemApi->suspend)(backgroundTask);
}

static Component *clockAppGetActiveTile()
{
    switch (tileNo)
    {
    case 1:
        return setTimeTile;
    case 2:
        return setDateTile;
    default:
        return clockTile;
    }
}

static void renderApp(bool forced)
{
    tiler->renderApp(forced);
}

static void setNextTile()
{
    if (tileNo < 2)
    {
        tileNo++;
    }
}

static void setPrevTile()
{
    if (tileNo > 0)
    {
        tileNo--;
    }
}

static void switchTile(Gesture gesture)
{
    if (gesture == MOVE_RIGHT)
    {
        setPrevTile();
    }
    else
    {
        setNextTile();
    }
    renderApp(true);
}

static void onGesture(Gesture gesture)
{
    bool horizontal = (gesture == MOVE_LEFT) || (gesture == MOVE_RIGHT);
    if (horizontal)
    {
        switchTile(gesture);
    }
    else
    {
        (manager->switchApp)(gesture == MOVE_UP); // todo should be manager->onGesture let manager decide how to handle #126
    }
}

static Date getDate()
{
    return date;
}

static void setTime(unsigned char hour, unsigned char minute)
{
    Date adjustedDate = {
        .year = date.year,
        .month = date.month,
        .day = date.day,
        .hour = hour,
        .minute = minute,
        .second = 0,
    };
    rtcApi->setDate(adjustedDate);
}

static void setDate(unsigned char day, unsigned char month)
{
    Date adjustedDate = {
        .year = date.year,
        .month = month,
        .day = day,
        .hour = date.hour,
        .minute = date.minute,
        .second = 0,
    };
    rtcApi->setDate(adjustedDate);
}

static int getBattery()
{
    return batteryPercent;
}

static unsigned int getStepCounter()
{
    return stepCounter;
}

static unsigned long getNextWakeUpPeriod()
{
    return NW_DONT_CARE;
}

void clockAppTick(unsigned long tick)
{
    update();
    renderApp(false);
}

App createClockApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, RtcApi *rtcApiRef, PowerApi *powerApiRef, BmaApi *bmaApiRef, SupervisorApi *supervisorApiRef, Tiler *tilerRef, Manager *managerRef, Factory *factory)
{
    rtcApi = rtcApiRef;
    powerApi = powerApiRef;
    bmaApi = bmaApiRef,
    tiler = tilerRef;
    manager = managerRef;
    backgroundTask = backgroundTaskHandleRef;
    systemApi = systemApiRef;
    supervisorApi = supervisorApiRef; 

    tileNo = 0;

    date = {
        // todo create const #125
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
        .setTime = setTime,
        .setDate = setDate,
        .getBattery = getBattery,
        .getStepCounter = getStepCounter,
        .getTimeToSleep = supervisorApi->getTimeToSleep,
        .getNextWakeUpPeriod = supervisorApi->getNextWakeUpPeriod,
        .getWakeUpReason = supervisorApi->getWakeUpReason,
        .onGesture = onGesture,
        .getTickCount = systemApi->getTickCount,
    };

    clockTile = createClockAppTile(&api, factory);
    setTimeTile = createClockAppSetTimeTile(&api, factory);
    setDateTile = createClockAppSetDateTile(&api, factory);

    return {
        .activate = clockAppActivate,
        .deactivate = clockAppDeactivate,
        .getActiveTile = clockAppGetActiveTile,
        .getNextWakeUpPeriod = getNextWakeUpPeriod,
    };
}