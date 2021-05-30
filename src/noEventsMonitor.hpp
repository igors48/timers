#include <freertos.hpp>

typedef struct
{
    void *lastTouchTimestampMutex;
    long *lastTouchTimestamp;
    void *backlightLevelMutex;
    unsigned char *backlightLevel;
    Take take;
    Give give;
    Time time;
    Log log;

} NoEventsMonitorParameters;

void noEventsMonitor(NoEventsMonitorParameters *p);