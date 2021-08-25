#include "component.hpp"

void componentNoopHandler(Component *component, signed short x, signed short y)
{
    // empty
}

bool componentNewState(Component component, WatchState watchState)
{
    return true;
}