#include "buttonListener.hpp"

void buttonListener(ButtonListenerParameters *p)
{
    p->powerApi->readIRQ();
    if (p->powerApi->isPEKShortPressIRQ())
    {
        if (p->systemApi->take(p->lastUserEventTimestampMutex, 100))
        {
            *p->lastUserEventTimestamp = p->systemApi->time();
            p->systemApi->give(p->lastUserEventTimestampMutex);
            p->systemApi->log("buttonListener", "PowerKey Press");
        }
    }
    p->powerApi->clearIRQ();
}
