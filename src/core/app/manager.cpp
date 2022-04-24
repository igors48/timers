#include "manager.hpp"

static Tiler *tiler;
static void **managedApps;
static unsigned char managedAppsCount;
static unsigned char activeAppIndex;

static void activateApp(unsigned char index)
{
    activeAppIndex = index;
    App *app = (App *)managedApps[activeAppIndex];
    (app->activate)(app);
    (tiler->setApp)(app);
    (tiler->renderApp)(true);
}

Manager createManager(unsigned char appsCount, void **apps, Tiler *tiler)
{
    managedApps = apps;
    managedAppsCount = appsCount;

    return {
        .activateApp = activateApp,
    };
}