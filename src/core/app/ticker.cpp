/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Ticker task implementation
*/
#include "ticker.hpp"

/**
 * @brief Calls configured function in a thread safe manner
 * 
 * @param v Pointer to the ticker parameters
 */
static void ticker(void *v) 
{
    TickerParameters *p = (TickerParameters *)v;
    if (p->systemApi->take(p->watchMutex, p->delayMs))
    {
        (p->func)();
        (p->systemApi->give)(p->watchMutex);
    } 
    else 
    {
        (p->systemApi->log)(p->name, "failed to take watch mutex");
    }
    (p->systemApi->delay)(p->delayMs);
}

void tickerTask(void *p)
{
    while (true)
    {
        ticker(p);
    }
}