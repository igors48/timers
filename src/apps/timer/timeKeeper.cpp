/**
 * @file 
 * @author Igor Usenko
 * @brief time keeper implementation
*/
#include "timeKeeper.hpp"

void timeKeeperTick(TimeKeeper *timeKeeper, unsigned int tickCount)
{
    unsigned int passed = tickCount - timeKeeper->lastTick;
    if (passed >= timeKeeper->duration)
    {
        timeKeeper->duration = 0;
    }
    else
    {
        timeKeeper->duration -= passed;
    }
    timeKeeper->lastTick = tickCount;
}

void timeKeeperReset(TimeKeeper *timeKeeper, unsigned int duration, unsigned int tickCount)
{
    timeKeeper->duration = duration;
    timeKeeper->lastTick = tickCount;
}

TimeKeeper timeKeeperCreate()
{
    return {
        .duration = 0,
        .lastTick = 0,
    };
}