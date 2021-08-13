#include "buttonListener.hpp"

void buttonListener(ButtonListenerParameters *p)
{
    if (p->systemApi->take(p->watchMutex, 100))
    {
        p->powerApi->readIRQ();
        if (p->powerApi->isPEKShortPressIRQ())
        {
            p->systemApi->log("buttonListener", "PowerKey Press");
            *p->lastUserEventTimestamp = p->systemApi->time(); // todo consider remove it. supervisor updates this timestamp
        }
        p->powerApi->clearIRQ();
        p->systemApi->give(p->watchMutex);
    }
}
