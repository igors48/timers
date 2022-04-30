#include "manager.hpp"

static Tiler *tiler;
static void **managedApps;
static unsigned char managedAppsCount;
static unsigned char activeAppIndex;

static void activateApp(unsigned char index)
{
    if (activeAppIndex != 255)
    {
        App *currentApp = (App *)managedApps[activeAppIndex];
        (currentApp->deactivate)(currentApp);
    }
    activeAppIndex = index;
    App *app = (App *)managedApps[activeAppIndex];
    (app->activate)(app);
    (tiler->setApp)(app);
    (tiler->renderApp)(true);
}

Manager createManager(unsigned char appsCount, void **apps, Tiler *tilerRef)
{
    managedApps = apps;
    managedAppsCount = appsCount;
    tiler = tilerRef;

    activeAppIndex = 255;

    return {
        .activateApp = activateApp,
    };
}