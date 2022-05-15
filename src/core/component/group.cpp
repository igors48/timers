#include <stddef.h>

#include <LilyGoWatch.h>

#include "group.hpp"

void groupRender(Component *group, bool forced, TftApi *tftApi)
{
    GroupState *state = (GroupState *)(group->state);
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        bool needRender = (current->newState)(current);
        if (forced || needRender)
        {
            (current->render)(current, true/*forced*/, tftApi); // todo fix this quick hack for non idempotent newState
        }
    }
}

bool groupNewState(Component *component)
{
    GroupState *state = (GroupState *)(component->state);
    bool result = false;
    for (int i = 0; i < state->childrenCount; i++)
    {
        Component *current = (Component *)(state->children[i]);
        if ((current->newState)(current))
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
        Component *target = (current->contains)(current, x, y); // todo update test
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
        Serial.printf("mount %d\r\n", i);
        Component *current = (Component *)(state->children[i]);
        Serial.printf("mount current %d %p\r\n", i, (void *)current);
        (current->mount)(current, component->x, component->y);
        Serial.printf("after mount current %d\r\n", i);
    }
}

