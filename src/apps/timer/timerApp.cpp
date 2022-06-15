#include "timerApp.hpp"
#include "timer.hpp"

static void *backgroundTask;
static SystemApi *systemApi;
static SoundApi *soundApi;
static Manager *manager;
static Tiler *tiler;

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

static unsigned long getNextWakeUpPeriod()
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

static unsigned long getTimerState()
{
    switch (timer.state)
    {
        case TMS_RUN:
            return timer.timeKeeper.duration / 1000; // todo replace two dots access with function
        default:
            return 0;        
    }
}

static unsigned long getSimulatedTickCount() // todo create time service with milliseconds simulation based on freertos ticks
{
    const long time = (systemApi->time)();
    return time * 1000;
}


static void startTimer()
{
    const unsigned long tickCount = getSimulatedTickCount();
    timerStart(&timer, 15000, tickCount);
}

static void stopTimer()
{
    timerStop(&timer);
}

static void onGesture(Gesture gesture)
{
    const bool horizontal = (gesture == MOVE_LEFT) || (gesture == MOVE_RIGHT);
    if (!horizontal)
    {
        (manager->switchApp)(gesture == MOVE_UP);
    }
}

void timerAppTick()
{
    const unsigned long tickCount = getSimulatedTickCount();
    timerTick(&timer, tickCount);
    (tiler->renderApp)(false);
}

App createTimerApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, SoundApi *soundApiRef, Tiler *tilerRef, Manager *managerRef, Factory *factoryRef)
{
    backgroundTask = backgroundTaskHandleRef;
    systemApi = systemApiRef;
    soundApi = soundApiRef;
    manager = managerRef;
    tiler = tilerRef;

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
