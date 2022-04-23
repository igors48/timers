#include "manager.hpp"

static Tiler *tiler;
static void **apps;
static unsigned char appsCount;
static unsigned char activeAppIndex;

static void activateApp(unsigned char index)
{
    activeAppIndex = index;
    App *app = (App *)apps[activeAppIndex];
    (app->activate)(app);
    (tiler->setApp)(app);
}

static void activatePrevApp()
{
    // empty
}

static void activateNextApp()
{
    // empty
}

Manager createManager(unsigned char appsCount, void **apps, Tiler *tiler)
{
    return {

    };
}