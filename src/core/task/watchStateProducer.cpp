#include "watchStateProducer.hpp"

static const char WATCH_STATE_PRODUCER[] = "watchStateProducer";

void watchStateProducer(void *v)
{
    WatchStateProducerParameters *p = (WatchStateProducerParameters *)v;
    if (p->systemApi->take(p->watchMutex, 10)) {
        Date date = p->rtcApi->getDate();
        int battPercentage = p->powerApi->getBattPercentage();
        p->state->date.year = date.year;
        p->state->date.month = date.month;
        p->state->date.day = date.day;
        p->state->date.hour = date.hour;
        p->state->date.minute = date.minute;
        p->state->date.second = date.second;
        p->state->battPercentage = battPercentage;
        p->state->stepCount = p->bmaApi->getCounter();
        p->systemApi->give(p->watchMutex);    
    }
    else
    {
        p->systemApi->log(WATCH_STATE_PRODUCER, "failed to take watch mutex");
    }
}