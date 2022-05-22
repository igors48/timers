#include "toolsApp.hpp"

static Manager *manager;
static SoundApi *soundApi;

static ToolsAppApi api;

static Component *tile;

static void beep()
{

}
    
static void onGesture(Gesture gesture)
{

} 

static void onGesture(Gesture gesture)
{
    bool horizontal = (gesture == MOVE_LEFT) || (gesture == MOVE_RIGHT);
    if (!horizontal)
    {
        (manager->switchApp)(gesture == MOVE_UP);
    }
}

App createClockApp(SoundApi *soundApiRef, Manager *managerRef, Factory *factoryRef)
{
    manager = managerRef;
    soundApi = soundApi;

    api = {};

    tile = createToolsAppTile(&api, factoryRef);
}