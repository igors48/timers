#include "timerApp.hpp"
#include "timer.hpp"

static void *backgroundTask;
static SystemApi *systemApi;
static SoundApi *soundApi;
static Manager *manager;

static Timer timer;
static Component *tile;

static TimerAppApi api;

static void activate()
{
    (systemApi->resume)(backgroundTask);
}

static void deactivate()
{
}

static Component *getActiveTile()
{
    return tile;
}

static unsigned int getNextWakeUpPeriod()
{
    return NW_DONT_CARE;
}

void timerAppTick()
{

}

App createTimerApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, SoundApi *soundApiRef, Manager *managerRef, Factory *factoryRef)
{
    backgroundTask = backgroundTaskHandleRef;
    systemApi = systemApiRef;
    soundApi = soundApiRef;
    manager = managerRef;

    timer = timerCreate(soundApi);

    api = {};
    
    tile = createTimerAppTile(&api, factoryRef);

    return {
        .activate = activate,
        .deactivate = deactivate,
        .getActiveTile = getActiveTile,
        .getNextWakeUpPeriod = getNextWakeUpPeriod,
    };
}
