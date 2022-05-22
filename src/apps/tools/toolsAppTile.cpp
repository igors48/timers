#include "toolsApp.hpp"

static char BEEP[] = "BEEP";

static const unsigned char COMPONENTS_COUNT = 1;
static void* components[COMPONENTS_COUNT];

static ToolsAppApi *api;

static void beep(void *c)
{
    api->beep();
}

static void onGesture(Component *component, Gesture gesture)
{
    api->onGesture(gesture);
}

Component* createToolsAppTile(ToolsAppApi *toolsAppApi, Factory *factory)
{
    api = toolsAppApi;

    ButtonComponentState* beepButtonState = (factory->createButtonStateRef)(BEEP, EG_ONCE, beep);
    
    components[0] = (factory->createButtonComponentRef)(160, 20, 66, 50, beepButtonState);
 
    GroupState* state = (factory->createGroupStateRef)(COMPONENTS_COUNT, components);
    Component* group = (factory->createGroupComponentRef)(0, 0, state);
    group->onGesture = onGesture; 

    group->mount(group, 0, 0);
    
    return group;
}