/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Implementation of the tick counter
*/
#include <climits>

#include "tickCounter.hpp"

/**
 * @brief Current tick value
 */
static unsigned long tickCount;

/**
 * @brief Last FreeRTOS tick value
 */
static unsigned int lastSystemTickCount;

/**
 * @brief FreeRTOS tick counter overflow counter
 */
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