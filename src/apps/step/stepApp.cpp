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

static void activate(App *app)
{
    update();
    (systemApi->resume)(backgroundTask);
}

static void deactivate(App *app)
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

static void switchApp()
{
    (manager->activateApp)(0);
}

static void onGesture(Gesture gesture)
{
    bool horizontal = (gesture == MOVE_LEFT) || (gesture == MOVE_RIGHT);
    if (!horizontal)
    {
        switchApp();
    }
}

void stepAppTick()
{
    update();
    render(false);
}

App createStepApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, BmaApi *bmaApiRef, Tiler *tilerRef, Manager *managerRef)
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
        .render = render,
    };

    stepTile = createStepAppTile(&api);

    return {
        .activate = activate,
        .deactivate = deactivate,
        .getActiveTile = getActiveTile,
    };
}
