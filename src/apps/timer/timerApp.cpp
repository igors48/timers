#include "timerApp.hpp"
#include "timer.hpp"

static SoundApi *soundApi;
static Manager *manager;

static Timer timer;

App createTimerApp(SoundApi *soundApiRef, Manager *managerRef, Factory *factoryRef)
{
    soundApi = soundApiRef;
    manager = managerRef;

    timer = timerCreate(soundApi);
}