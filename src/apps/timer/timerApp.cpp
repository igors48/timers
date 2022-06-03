#include "timerApp.hpp"
#include "timer.hpp"

static SoundApi *soundApi;
static Manager *manager;

static Timer timer;
static Component *tile;

static TimerAppApi api;

static void activate()
{
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

App createTimerApp(SoundApi *soundApiRef, Manager *managerRef, Factory *factoryRef)
{
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

Component* createTimerAppTile(TimerAppApi *toolsAppApi, Factory *factory)
{

}