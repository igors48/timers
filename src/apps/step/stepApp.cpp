#include "stepApp.hpp"

static void *backgroundTask;

static unsigned int counter;

static SystemApi *systemApi;
static BmaApi *bmaApi;
static Tiler *tiler;
static Manager *manager;

static Component *stepTile;

static StepAppApi api;

static void update()
{
    counter = (bmaApi->getCounter)();
}

static void activate()
{
    update();
    (systemApi->resume)(backgroundTask);
}

static void deactivate()
{
    (systemApi->suspend)(backgroundTask);
}

static Component* getActiveTile()
{
    return stepTile;
}

static unsigned int getStepCounter()
{
    return counter;
}
    
static void resetStepCounter()
{
    (bmaApi->resetStepCounter)();
}

static void render(bool forced)
{
    tiler->renderApp(forced);
}

static void onGesture(Gesture gesture)
{
    bool horizontal = (gesture == MOVE_LEFT) || (gesture == MOVE_RIGHT);
    if (!horizontal)
    {
        (manager->switchApp)(gesture == MOVE_UP);
    }
}

static unsigned int getNextWakeUpPeriod()
{
    return NW_DONT_CARE;
}

void stepAppTick()
{
    update();
    render(false);
}

App createStepApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, BmaApi *bmaApiRef, Tiler *tilerRef, Manager *managerRef, Factory *factory)
{
    backgroundTask = backgroundTaskHandleRef;
    systemApi = systemApiRef;
    bmaApi = bmaApiRef;
    tiler = tilerRef;
    manager = managerRef;

    counter = 0;

    api = {
        .getStepCounter = getStepCounter,
        .resetStepCounter = resetStepCounter,
        .onGesture = onGesture,
    };

    stepTile = createStepAppTile(&api, factory);

    return {
        .activate = activate,
        .deactivate = deactivate,
        .getActiveTile = getActiveTile,
        .getNextWakeUpPeriod = getNextWakeUpPeriod,
    };
}
