#include "tiler.hpp"

static App *activeApp;
static TftApi *tilerTftApi;

static void renderApp(bool forced)
{
    Component *activeTile = (activeApp->getActiveTile)();
    (activeTile->render)(activeTile, forced, tilerTftApi);
}

static void setApp(App *app)
{
    activeApp = app;
}

static Component* contains(Component *component, signed short x, signed short y)
{
    Component *activeTile = (activeApp->getActiveTile)();
    return (activeTile->contains)(activeTile, x, y);
}

static void onTouch(Component *component, signed short x, signed short y)
{
    (component->onTouch)(component, x, y);    
    renderApp(false);
}

static void onMove(Component *component, signed short x, signed short y)
{
    (component->onMove)(component, x, y);    
    renderApp(false);
}

static void onRelease(Component *component, signed short x, signed short y)
{
    (component->onRelease)(component, x, y);    
    renderApp(false);
}

static void onGesture(Component *component, Gesture gesture)
{
    (component->onGesture)(component, gesture);    
    renderApp(false);
}

Tiler createTiler(TftApi *tftApi)
{
    tilerTftApi = tftApi;

    return {
        .setApp = setApp,
        .renderApp = renderApp,
        .contains = contains,
        .onTouch = onTouch, 
        .onMove = onMove,  
        .onRelease = onRelease,  
        .onGesture = onGesture,  
    };
}
