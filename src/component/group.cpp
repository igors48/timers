#include <stddef.h>
#include "group.hpp"

/*
   TouchEventHandler onTouch; // todo consider interface EventListener 
    TouchEventHandler onMove;  
    TouchEventHandler onRelease;  
    Render render;  // todo consider interface Renderable
    NewState newState; // todo consider better name  -- always true

*/

void groupRender(Component *group, WatchState *watchState, TftApi *tftApi)
{
    GroupState *state = (GroupState *)(group->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        bool needRender = current->newState(current, watchState);
        if (needRender)
        {
            current->render(current, watchState, tftApi);
        }
    }
}

void groupOnTouch(Component *group, signed short x, signed short y)
{
    Component *target = findTarget(group, x, y);
    if (target != NULL)
    {
       signed short translatedX = x - group->x; 
       signed short translatedY = y - group->y;
       target->onTouch(target, translatedX, translatedY);
    }
}

Component* findTarget(Component *group, signed short x, signed short y) 
{
    GroupState *state = (GroupState *)(group->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        if ((x > current->x) && (x < current->x + current->w) && (y > current->y) && (y < current->y + current->h))
        {
            return current;
        }
    }
    return NULL;
}