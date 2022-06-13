/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief time keeper implementation
*/
#include "timeKeeper.hpp"

void timeKeeperTick(TimeKeeper *timeKeeper, unsigned long tickCount)
{
    const unsigned long passed = tickCount - timeKeeper->lastTick;
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

void timeKeeperReset(TimeKeeper *timeKeeper, unsigned long duration, unsigned long tickCount)
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