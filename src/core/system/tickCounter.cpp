#include <climits>

#include "tickCounter.hpp"

static unsigned long tickCount;
static unsigned int lastSystemTickCount;
static unsigned char overflowCount;

unsigned long newTickCountValue(unsigned int value)
{
    bool counterResetByOverflow = value < lastSystemTickCount;
    if (counterResetByOverflow)
    {
        overflowCount++;
    }
    lastSystemTickCount = value;
    tickCount = (unsigned long)overflowCount * (unsigned long)UINT_MAX + value;
    return tickCount;
}

void resetTickCount(unsigned int value)
{
    tickCount = value;
    lastSystemTickCount = value;
    overflowCount = 0;
}