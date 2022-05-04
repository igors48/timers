#include "tiler.hpp"

static App *activeApp;
static TftApi *tilerTftApi;

static void renderApp(bool forced)
{
    Component *activeTile = (activeApp->getActiveTile)();
    if (forced) 
    {
        (tilerTftApi->fillRect)(0, 0, 240, 240, COLOR_BLACK);
    } 
    (activeTile->render)(activeTile, forced, tilerTftApi);
}

static void setApp(App *app)
{
    activeApp = app;
}

static Component* contains(signed short x, signed short y)
{
    Component *activeTile = (activeApp->getActiveTile)();
    return (activeTile->contains)(activeTile, x, y);
}

static void onTouch(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    (component->onTouch)(component, x, y, tickCount);    
    renderApp(false);
}

static void onMove(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    (component->onMove)(component, x, y, tickCount);    
    renderApp(false);
}

static void onRelease(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    (component->onRelease)(component, x, y, tickCount);    
    renderApp(false);
}

static void onGesture(Gesture gesture)
{
    Component *activeTile = (activeApp->getActiveTile)(); 
    (activeTile->onGesture)(activeTile, gesture);  
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
