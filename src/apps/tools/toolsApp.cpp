#include "toolsApp.hpp"

static Manager *manager;
static SoundApi *soundApi;

static ToolsAppApi api;

static Component *tile;

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

static unsigned long getNextWakeUpPeriod()
{
    return NW_DONT_CARE;
}

static void beep()
{
    (soundApi->beep)();
}

static void onGesture(Gesture gesture)
{
    bool horizontal = (gesture == MOVE_LEFT) || (gesture == MOVE_RIGHT);
    if (!horizontal)
    {
        (manager->switchApp)(gesture == MOVE_UP);
    }
}

App createToolsApp(SoundApi *soundApiRef, Manager *managerRef, Factory *factoryRef)
{
    manager = managerRef;
    soundApi = soundApiRef;

    api = {
        .beep = beep,
        .onGesture = onGesture,
    };

    tile = createToolsAppTile(&api, factoryRef);

    return {
        .activate = activate,
        .deactivate = deactivate,
        .getActiveTile = getActiveTile,
        .getNextWakeUpPeriod = getNextWakeUpPeriod,
    };
}