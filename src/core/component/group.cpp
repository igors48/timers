/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Group of components implementation
*/
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

bool groupIsStateModified(Component *group)
{
    return true;
}

void groupUpdateState(Component *group)
{
    GroupState *state = (GroupState *)(group->state);
    bool result = false;
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        (current->updateState)(current);
    }
}

Component *groupContains(Component *group, signed short x, signed short y)
{
    GroupState *state = (GroupState *)(group->state);
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

void groupMount(Component *group, signed short x, signed short y)
{
    group->x += x;
    group->y += y;
    GroupState *state = (GroupState *)(group->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        (current->mount)(current, group->x, group->y);
    }
}

void groupOnTick(Component *group, unsigned long tick)
{
    GroupState *state = (GroupState *)(group->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        (current->onTick)(current, tick);
    }    
}
