#include "touchScreenMonitor.hpp"

static const char TOUCH_SCREEN_MONITOR[] = "touchScreenMonitor";

void touchScreenMonitor(void *p)
{
    TouchScreenMonitorParameters *t = (TouchScreenMonitorParameters *)p;
    unsigned char touched = t->getTouched();
    if (touched)
    {
        long timestamp = t->time();
        if (t->take(t->lastTouchTimestampMutex, 10))
        {
            *t->lastTouchTimestamp = timestamp;
            t->give(t->lastTouchTimestampMutex);
            t->log(TOUCH_SCREEN_MONITOR, "lastTouchTimestamp set to %lu \r\n", t->lastTouchTimestamp);
        }
        else
        {
            t->log(TOUCH_SCREEN_MONITOR, "lastTouchTimestampMutex couldnt obtain from touchScreenMonitor");
        }
    }
}
