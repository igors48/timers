#include "stepApp.hpp"

static void *backgroundTask;

static unsigned int counter;

static SystemApi *systemApi;
static BmaApi *bmaApi;
static Tiler *tiler;

static Component *stepTile;

static StepAppApi api;

static void activate(App *app)
{
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

static void onGesture(Gesture gesture)
{
}

void stepAppTick()
{
    counter = (bmaApi->getCounter)();
}

App createStepApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, BmaApi *bmaApiRef, Tiler *tilerRef)
{
    backgroundTask = backgroundTaskHandleRef;
    systemApi = systemApiRef;
    bmaApi = bmaApiRef;
    tiler = tilerRef;

    counter = 0;

    api = {
        .getStepCounter = getStepCounter,
        .resetStepCounter = resetStepCounter,
        .onGesture = onGesture,
        .render = render,
    };

    return {
        .activate = activate,
        .deactivate = deactivate,
        .getActiveTile = getActiveTile,
    };
}
