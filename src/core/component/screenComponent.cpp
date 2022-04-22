#include "screenComponent.hpp"
#include "component.hpp"
#include "group.hpp"

static Component* getActiveTile(Component *component)
{
    ScreenState *state = (ScreenState *)component->state;
    return (Component *)(state->tiles[state->activeTile]);
}

static void screenComponentRender(Component *component, bool forced, TftApi *tftApi)
{
    Component* active = getActiveTile(component);
    bool activeTileChanged = (component->newState)(component);
    bool forcedRender = forced || activeTileChanged;
    if (forcedRender) 
    {
        (tftApi->fillRect)(0, 0, 240, 240, COLOR_BLACK);
    } 
    (active->render)(active, forcedRender, tftApi);
}

static bool screenComponentNewState(Component *component)
{
    ScreenState *state = (ScreenState *)component->state;
    if (state->activeTile != state->_activeTile) { 
        state->_activeTile = state->activeTile;
        return true;
    }
    return false;
}

static Component* screenComponentContains(Component *component, signed short x, signed short y)
{
    Component* active = getActiveTile(component);
    return (active->contains)(active, x, y);
}

static void screenComponentMount(Component *component, signed short x, signed short y)
{
    ScreenState *state = (ScreenState *)component->state;
    for (int i = 0; i < state->tilesCount; i++)
    {
        Component *current = (Component *)(state->tiles[i]);
        (current->mount)(current, 0, 0);
    }
}

static void screenComponentGestureEventHandler(Component *component, Gesture gesture)
{
    Component* active = getActiveTile(component);
    (active->onGesture)(active, gesture);
}

ScreenState createScreenState(unsigned short tilesCount, void **tiles)
{
    return {
        .tilesCount = tilesCount,
        .tiles = tiles,
        .activeTile = 0,
        ._activeTile = 65535,
    };    
}

Component createScreenComponent(ScreenState *state)
{
    return {
        .x = 0,
        .y = 0,
        .w = 0,
        .h = 0,
        .contains = screenComponentContains,
        .mount = screenComponentMount,
        .onTouch = componentNoopHandler,
        .onMove = componentNoopHandler,
        .onRelease = componentNoopHandler,
        .onGesture = screenComponentGestureEventHandler,
        .render = screenComponentRender,
        .newState = screenComponentNewState,
        .state = state,
    };
}