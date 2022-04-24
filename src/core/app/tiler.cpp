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

Tiler createTiler(TftApi *tftApi)
{
    tilerTftApi = tftApi;

    return {
        .setApp = setApp,
        .renderApp = renderApp,
    };
}
