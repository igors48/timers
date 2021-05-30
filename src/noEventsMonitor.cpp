#include <noEventsMonitor.hpp>

static const char NO_EVENTS_MONITOR[] = "noEventsMonitor";

void noEventsMonitor(NoEventsMonitorParameters *p)
{
    if (p->take(p->lastTouchTimestampMutex, 10))
    {
        long last = *p->lastTouchTimestamp;
        p->give(p->lastTouchTimestampMutex);
        long current = p->time();
        long diff = current - last;
        unsigned char level = 0;
        if (diff < 5)
        {
            level = 128;
        }
        else
        {
            level = 8;
        }
        if (p->take(p->backlightLevelMutex, 10))
        {
            *p->backlightLevel = level;
            p->give(p->backlightLevelMutex);
            p->log(NO_EVENTS_MONITOR, "backlightLevel set to %d \r\n", level);
        }
        else
        {
            p->log(NO_EVENTS_MONITOR, "failed to take backlightLevelMutex");
        }
    }
    else
    {
        p->log(NO_EVENTS_MONITOR, "failed to take lastTouchTimestampMutex");
    }
}

