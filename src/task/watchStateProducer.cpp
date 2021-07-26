#include "watchStateProducer.hpp"

void watchStateProducer(void *v)
{
    WatchStateProducerParameters *p = (WatchStateProducerParameters *)v;
    p->systemApi->log("watchStateProducer", "watchStateProducer");
    Date date = p->rtcApi->getDate();
    if (p->systemApi->take(p->stateMutex, 10)) {
        p->state->date.year = date.year;
        p->state->date.month = date.month;
        p->state->date.day = date.day;
        p->state->date.hour = date.hour;
        p->state->date.minute = date.minute;
        p->state->date.second = date.second;
        p->systemApi->give(p->stateMutex);    
    }
}