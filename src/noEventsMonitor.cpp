#include <noEventsMonitor.hpp>

static const char NO_EVENTS_MONITOR[] = "noEventsMonitor";

void _setBacklightLevel(unsigned char level, NoEventsMonitorParameters *p)
{
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

void noEventsMonitor(NoEventsMonitorParameters *p)
{
    if (p->take(p->lastTouchTimestampMutex, 10))
    {
        long last = *p->lastTouchTimestamp;
        p->give(p->lastTouchTimestampMutex);
        long current = p->time();
        long diff = current - last;
        if (diff < 5)
        {
            _setBacklightLevel(128, p);
        }
        else if (diff > 5 && diff < 10)
        {
            _setBacklightLevel(8, p);
        }
        else
        {
            p->log(NO_EVENTS_MONITOR, "go to light sleep");
            p->sleep();
        }
    }
    else
    {
        p->log(NO_EVENTS_MONITOR, "failed to take lastTouchTimestampMutex");
    }
}
