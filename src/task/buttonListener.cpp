#include "buttonListener.hpp"

void buttonListener(ButtonListenerParameters *p)
{
    p->powerApi->readIRQ();
    if (p->powerApi->isPEKShortPressIRQ())
    {
        if (p->systemApi->take(p->lastShortPressTimestampMutex, 100))
        {
            *p->lastShortPressTimestamp = p->systemApi->time();
            p->systemApi->give(p->lastShortPressTimestampMutex);
            p->systemApi->log("buttonListener", "PowerKey Press");
        }
    }
    p->powerApi->clearIRQ();
}
