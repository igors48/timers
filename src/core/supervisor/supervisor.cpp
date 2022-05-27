#include "supervisor.hpp"
#include "core/tools/tools.hpp"

#define uS_TO_S_FACTOR 1000000 // Conversion factor for micro seconds to seconds
#define SLEEP_TIME_TRESHOLD 3

static const char SUPERVISOR[] = "supervisor";

static unsigned long calcSleepTime(SupervisorParameters *p)
{
    unsigned int secondsToNextWakeUp = (p->manager->getNextWakeUpPeriod)();
    if (secondsToNextWakeUp == NW_NO_SLEEP)
    {
        return NW_NO_SLEEP;
    }
    Date now = p->rtcApi->getDate();
    unsigned int secondsToNextHour = secondsToNextHourStart(now);
    unsigned int minimum = secondsToNextHour;
    if (secondsToNextWakeUp < secondsToNextHour)
    {
        minimum = secondsToNextWakeUp;
    }
    return minimum;
}

static void tryToSleep(SupervisorParameters *p)
{
    unsigned long sleepTime = calcSleepTime(p);
    if (sleepTime != NW_NO_SLEEP)
    {
        sleepTime = sleepTime - SLEEP_TIME_TRESHOLD; // todo consinder the new parameter
        if (sleepTime > 1)
        {
            p->supervisorSleep(p, sleepTime);
        }
    }
    *p->lastUserEventTimestamp = p->systemApi->time();
}

static bool timeToSleep(SupervisorParameters *p)
{
    long lastEventTimestamp = *p->lastUserEventTimestamp;
    long current = p->systemApi->time();
    long diff = current - lastEventTimestamp;
    p->systemApi->log(SUPERVISOR, "diff %d", diff);
    return diff >= p->goToSleepTime;
}

void supervisorSleep(void *v, unsigned int sleepTimeSec)
{
    SupervisorParameters *p = (SupervisorParameters *)v;
    p->systemApi->log(SUPERVISOR, "sleep for %d sec", sleepTimeSec);
    p->watchApi->beforeGoToSleep();
    p->watchApi->goToSleep(sleepTimeSec * uS_TO_S_FACTOR); // here it stops
    p->watchApi->afterWakeUp();
    p->systemApi->log(SUPERVISOR, "after wake up");
}

void supervisor(SupervisorParameters *p)
{
    if (p->systemApi->take(p->watchMutex, 10)) // todo there was missprint lastEventTimestamp vs lastEventTimestampMutex - tests dont see it
    {
        if (timeToSleep(p))
        {
            tryToSleep(p);
        }
        p->systemApi->give(p->watchMutex);
    }
    else
    {
        p->systemApi->log(SUPERVISOR, "failed to take watch mutex");
    }
}
