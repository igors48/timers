#include "clockApp.hpp"

static void clockAppActivate(App *app)
{
    ClockAppState *appState = (ClockAppState *)app->state;
    (app->systemApi->resume)(appState->backgroundTaskHandle);
}

static void clockAppDeactivate(App *app)
{
    ClockAppState *appState = (ClockAppState *)app->state;
    (app->systemApi->resume)(appState->backgroundTaskHandle);    
}

App createClockApp()
{
    return {
        .activate = clockAppActivate,
        .deactivate = clockAppDeactivate,
    };
}