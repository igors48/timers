#include "supervisor.hpp"
#include "core/tools/tools.hpp"

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define SLEEP_TIME_TRESHOLD 3 

static const char SUPERVISOR[] = "supervisor";

void supervisorSleep(void *v, unsigned short sleepTimeSec)
{
    SupervisorParameters *p = (SupervisorParameters *)v;
    p->systemApi->log(SUPERVISOR, "sleep for %d sec", sleepTimeSec);
    p->watchApi->beforeGoToSleep();
    p->watchApi->goToSleep(sleepTimeSec * uS_TO_S_FACTOR); // here it stops
    p->watchApi->afterWakeUp();
    p->systemApi->log(SUPERVISOR, "after wake up");
}

unsigned long calcSleepTime(SupervisorParameters *p)
{
    Date now = p->rtcApi->getDate();
    return secondsToNextHourStart(now);
}

void supervisor(SupervisorParameters *p)
{
    if (p->systemApi->take(p->watchMutex, 10)) // todo there was missprint lastEventTimestamp vs lastEventTimestampMutex - tests dont see it
    {
        long lastEventTimestamp = *p->lastUserEventTimestamp;
        long current = p->systemApi->time();
        long diff = current - lastEventTimestamp;
        p->systemApi->log(SUPERVISOR, "diff %d", diff);
        bool sleep = diff >= p->goToSleepTime; // todo extract the logic from here
        if (sleep)
        {
            unsigned long sleepTime = calcSleepTime(p);
            sleepTime = sleepTime - SLEEP_TIME_TRESHOLD;// todo consinder the new parameter
            if (sleepTime > 1)
            {
                p->supervisorSleep(p, sleepTime);
            }
            *p->lastUserEventTimestamp = p->systemApi->time();
        }
        p->systemApi->give(p->watchMutex);
    }
    else
    {
        p->systemApi->log(SUPERVISOR, "failed to take watch mutex");
    }
}
