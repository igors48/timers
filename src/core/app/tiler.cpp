#include "tiler.hpp"

static App *activeApp;
static Component *activeTile;
static TftApi *tilerTftApi;

static void renderTile()
{
    (activeTile->render)(activeTile, false, tilerTftApi);
}

static void forcedRenderTile()
{
    (activeTile->render)(activeTile, true, tilerTftApi);
}

static void setApp(App *app)
{
    activeApp = app;
    activeTile = (activeApp->getActiveTile)();
    forcedRenderTile();
}

static void activatePrevTile()
{
    activeTile = (activeApp->getPrevTile)();
    forcedRenderTile();
}

static void activateNextTile()
{
    activeTile = (activeApp->getNextTile)();
    forcedRenderTile();
}

Tiler createTiler(TftApi *tftApi)
{
    tilerTftApi = tftApi;

    return {
        .setApp = setApp,
        .activatePrevTile = activatePrevTile,
        .activateNextTile = activateNextTile,
        .renderTile = renderTile,
    };
}
