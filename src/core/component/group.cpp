#include <stddef.h>

#include "group.hpp"

void groupRender(Component *group, bool forced, TftApi *tftApi)
{
    GroupState *state = (GroupState *)(group->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        bool needRender = (current->isStateModified)(current);
        if (forced || needRender)
        {
            (current->render)(current, forced, tftApi);
        }
    }
}

bool groupIsStateModified(Component *component)
{
    GroupState *state = (GroupState *)(component->state);
    bool result = false;
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        if ((current->isStateModified)(current))
        {
            result = true;
        }
    }
    return result;
}

void groupUpdateState(Component *component)
{
    GroupState *state = (GroupState *)(component->state);
    bool result = false;
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        (current->updateState)(current);
    }
}

Component *groupContains(Component *component, signed short x, signed short y)
{
    GroupState *state = (GroupState *)(component->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        Component *target = (current->contains)(current, x, y);
        if (target != NULL)
        {
            return target;
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
