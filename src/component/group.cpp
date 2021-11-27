#include <stddef.h>

#include "group.hpp"

void groupRender(Component *group, bool forced, WatchState *watchState, TftApi *tftApi)
{
    GroupState *state = (GroupState *)(group->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        bool needRender = (current->newState)(current, watchState);
        if (forced || needRender)
        {
            (current->render)(current, forced, watchState, tftApi);
        }
    }
}

bool groupNewState(Component *component, WatchState *watchState)
{
    GroupState *state = (GroupState *)(component->state);
    bool result = false;
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        if ((current->newState)(current, watchState))
        {
            result = true;
        }
    }
    return result;
}

Component* groupContains(Component *component, signed short x, signed short y)
{
    GroupState *state = (GroupState *)(component->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        if ((current->contains)(current, x, y))
        {
            return current;
        }
    }    
    return NULL;
}

void groupMount(Component *component, signed short x, signed short y)
{
    component->x += x;
    component->y += y;
    GroupState *state = (GroupState *)(component->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        (current->mount)(current, component->x, component->y);
    }
}

GroupState createGroupState(unsigned char childrenCount, void **children)
{
    return {
        .childrenCount = childrenCount,
        .children = children,
    };
}

Component createGroupComponent(signed short x, signed short y, GroupState *state)
{
    return {
            .x = x,
            .y = y,
            .w = 0,
            .h = 0,
            .contains = groupContains,
            .mount = groupMount,
            .onTouch = componentNoopHandler,
            .onMove = componentNoopHandler,
            .onRelease = componentNoopHandler,
            .onGesture = componentGestureNoopHandler,
            .render = groupRender,
            .newState = groupNewState,
            .state = state,
        };
}
