#include "touchScreenMonitor.hpp"

static const char TOUCH_SCREEN_MONITOR[] = "touchScreenMonitor";

void touchScreenMonitor(TouchScreenMonitorParameters *p)
{
    unsigned char touched = p->getTouched();
    if (touched)
    {
        long timestamp = p->time();
        if (p->take(p->lastTouchTimestampMutex, 10))
        {
            *p->lastTouchTimestamp = timestamp;
            p->give(p->lastTouchTimestampMutex);
            p->log(TOUCH_SCREEN_MONITOR, "lastTouchTimestamp set to %lu \r\n", p->lastTouchTimestamp);
        }
        else
        {
            p->log(TOUCH_SCREEN_MONITOR, "lastTouchTimestampMutex couldnt obtain from touchScreenMonitor");
        }
    }
}
