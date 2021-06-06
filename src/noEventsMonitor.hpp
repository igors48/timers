#include "freertos.hpp"
#include "twatch.hpp"

typedef struct
{
    void *lastTouchTimestampMutex;
    long *lastTouchTimestamp;
    void *backlightLevelMutex;
    unsigned char *backlightLevel;
    LightSleep lightSleep;
    Take take;
    Give give;
    Time time;
    Log log;

} NoEventsMonitorParameters;

void noEventsMonitor(NoEventsMonitorParameters *p);