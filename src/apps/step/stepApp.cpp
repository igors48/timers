#include "stepApp.hpp"

static void *backgroundTask;

static unsigned int counter;

static SystemApi *systemApi;
static BmaApi *bmaApi;

static void clockAppActivate(App *app)
{
    (systemApi->resume)(backgroundTask);
}

static void clockAppDeactivate(App *app)
{
    (systemApi->suspend)(backgroundTask);
}

void stepAppTick()
{
    counter = (bmaApi->getCounter)();
}

App createStepApp(void *backgroundTaskHandleRef, SystemApi *systemApiRef, BmaApi *bmaApi, Tiler *tilerRef)
{

}
