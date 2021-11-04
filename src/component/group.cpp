#include <stddef.h>

#include "group.hpp"

void groupRender(signed short x, signed short y, Component *group, WatchState *watchState, TftApi *tftApi)
{
    GroupState *state = (GroupState *)(group->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        bool needRender = current->newState(current, watchState);
        if (needRender)
        {
            current->render(group->x + x, group->y + y, current, watchState, tftApi);
        }
    }
}

bool groupNewState(Component *component, WatchState *watchState)
{
    GroupState *state = (GroupState *)(component->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        if (current->newState(current, watchState))
        {
            return true;
        }
    }
    return false;
}

Component* groupContains(Component *component, signed short x, signed short y)
{
    GroupState *state = (GroupState *)(component->state);
    signed short translatedX = x - component->x; 
    signed short translatedY = y - component->y;
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        if (current->contains(current, translatedX, translatedY))
        {
            return current;
        }
    }    
    return NULL;
}

Component createGroupComponent(signed short x, signed short y, GroupState *state)
{
return {
        .x = x,
        .y = y,
        .w = 0,
        .h = 0,
        .contains = groupContains,
        .onTouch = componentNoopHandler,
        .onMove = componentNoopHandler,
        .onRelease = componentNoopHandler,
        .render = groupRender,
        .newState = groupNewState,
        .state = state,
    };
}
