#include "buttonListener.hpp"

void buttonListener(ButtonListenerParameters *p)
{
    if (p->systemApi->take(p->lastUserEventTimestampMutex, 100))
    {
        p->powerApi->readIRQ();
        if (p->powerApi->isPEKShortPressIRQ())
        {
            p->systemApi->log("buttonListener", "PowerKey Press");
            *p->lastUserEventTimestamp = p->systemApi->time();
        }
        p->powerApi->clearIRQ();
        p->systemApi->give(p->lastUserEventTimestampMutex);
    }
}
