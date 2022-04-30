#include "manager.hpp"

static const unsigned char NO_CURRENT_APP = 255;

static Tiler *tiler;
static void **managedApps;
static unsigned char managedAppsCount;
static unsigned char activeAppIndex;

static void activateApp(unsigned char index)
{
    if (activeAppIndex != NO_CURRENT_APP)
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

static void activateNextApp()
{
    unsigned char nextAppIndex = activeAppIndex + 1;
    if (nextAppIndex == managedAppsCount)
    {
        nextAppIndex = 0;
    }
    activateApp(nextAppIndex);
}

static void activatePrevApp()
{
    signed short prevAppIndex = activeAppIndex - 1;
    if (prevAppIndex < 0)
    {
        prevAppIndex = managedAppsCount - 1;
    }
    activateApp(prevAppIndex);
}

static void switchApp(bool next) 
{
    if (next)
    {
        activateNextApp();
    }
    else
    {
        activatePrevApp();
    }
}

Manager createManager(unsigned char appsCount, void **apps, Tiler *tilerRef)
{
    managedApps = apps;
    managedAppsCount = appsCount;
    tiler = tilerRef;

    activeAppIndex = NO_CURRENT_APP;

    return {
        .activateApp = activateApp,
        .switchApp = switchApp,
    };
}