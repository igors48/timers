#include "component.hpp"

void componentNoopHandler(Component component)
{
    // empty
}

bool componentNewState(Component component, WatchState watchState)
{
    return true;
}