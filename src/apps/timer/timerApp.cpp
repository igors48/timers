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
    switch (timer.state)
    {
        case TMS_RUN:
            return timer.timeKeeper.duration; // todo replace two dots access with function
        case TMS_ALARM:
            return NW_NO_SLEEP;
        default:
            return NW_DONT_CARE;        
    }
}

static unsigned int getTimerState()
{

}

static void startTimer()
{

}

static void stopTimer()
{

}

static void onGesture(Gesture gesture)
{

}

void timerAppTick()
{
    const unsigned int tickCount = (systemApi->getTickCount)();
    timerTick(&timer, tickCount);
}

App createTimerApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, SoundApi *soundApiRef, Manager *managerRef, Factory *factoryRef)
{
    backgroundTask = backgroundTaskHandleRef;
    systemApi = systemApiRef;
    soundApi = soundApiRef;
    manager = managerRef;

    timer = timerCreate(soundApi);

    api = {
        .getTimerState = getTimerState,
        .startTimer = startTimer,
        .stopTimer = stopTimer,
        .onGesture = onGesture, 
    };
    
    tile = createTimerAppTile(&api, factoryRef);

    return {
        .activate = activate,
        .deactivate = deactivate,
        .getActiveTile = getActiveTile,
        .getNextWakeUpPeriod = getNextWakeUpPeriod,
    };
}
