#include "tiler.hpp"

static App *activeApp;
static TftApi *tilerTftApi;
static bool developmentMode;

static void drawGrid()
{
    for (int i = 0; i <= 240; i += 10)
    {
        (tilerTftApi->drawFastHLine)(0, i, 240, 0x10A2);
        (tilerTftApi->drawFastVLine)(i, 0, 240, 0x10A2); // 20, 20, 20
    }
}

static void renderApp(bool forced)
{
    Component *activeTile = (activeApp->getActiveTile)();
    if (forced)
    {
        (tilerTftApi->fillRect)(0, 0, 240, 240, COLOR_BLACK);
        if (developmentMode)
        {
            drawGrid();
        }
    }
    (activeTile->render)(activeTile, forced, tilerTftApi);
    (activeTile->updateState)(activeTile);
}

static void setApp(App *app)
{
    activeApp = app;
}

static Component *contains(signed short x, signed short y)
{
    Component *activeTile = (activeApp->getActiveTile)();
    return (activeTile->contains)(activeTile, x, y);
}

static void onTouch(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    (component->onTouch)(component, x, y, tickCount);
    renderApp(false);
}

static void onMove(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    (component->onMove)(component, x, y, tickCount);
    renderApp(false);
}

static void onRelease(Component *component, signed short x, signed short y, unsigned long tickCount)
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

static void onButton()
{
    developmentMode = !developmentMode;
    Component *activeTile = (activeApp->getActiveTile)();
    (activeTile->onButton)(activeTile);
    renderApp(true /*false*/);
}

Tiler createTiler(TftApi *tftApi)
{
    tilerTftApi = tftApi;

    developmentMode = false;

    return {
        .setApp = setApp,
        .renderApp = renderApp,
        .contains = contains,
        .onTouch = onTouch,
        .onMove = onMove,
        .onRelease = onRelease,
        .onGesture = onGesture,
        .onButton = onButton,
    };
}
