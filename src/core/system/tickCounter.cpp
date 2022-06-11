#include <climits>

#include "tickCounter.hpp"

static unsigned long tickCount;
static unsigned int lastSystemTickCount;

unsigned long newTickCountValue(unsigned int value)
{
    bool counterResetByOverflow = value < lastSystemTickCount;
    unsigned int delta;
    if (counterResetByOverflow)
    {
        delta = UINT_MAX - lastSystemTickCount + value;
    }
    else
    {
        delta = value - lastSystemTickCount;
    }
    tickCount += delta;
    lastSystemTickCount = value;

    return tickCount;
}

void resetTickCount(unsigned int value)
{
    tickCount = value;
    lastSystemTickCount = value;
}