#include "timeKeeper.hpp"

void timeKeeperTick(TimeKeeper *timeKeeper, unsigned int tickCount)
{
    unsigned int passed = tickCount - timeKeeper->lastTick;
    if (passed >= timeKeeper->counter)
    {
        timeKeeper->counter = 0;
    }
    else
    {
        timeKeeper->counter -= passed;
    }
    timeKeeper->lastTick = tickCount;
}

void timeKeeperReset(TimeKeeper *timeKeeper, unsigned int counter, unsigned int tickCount)
{
    timeKeeper->counter = counter;
    timeKeeper->lastTick = tickCount;
}

TimeKeeper timeKeeperCreate()
{
    return {
        .counter = 0,
        .lastTick = 0,
    };
}