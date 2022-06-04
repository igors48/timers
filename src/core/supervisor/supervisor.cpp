#include "supervisor.hpp"
#include "core/tools/tools.hpp"

#define uS_TO_S_FACTOR 1000000 // Conversion factor for micro seconds to seconds
#define SLEEP_TIME_TRESHOLD 3

static const char SUPERVISOR[] = "supervisor";

static unsigned int cachedNextWakeUpPeriod = 0;
static unsigned short cachedTimeToSleep = 0;
static WakeUpReason cachedWakeUpReason = WUR_UNKNOWN;

static unsigned int getNextWakeUpPeriod()
{
    return cachedNextWakeUpPeriod;
}

static unsigned short getTimeToSleep()
{
    return cachedTimeToSleep;
}

static WakeUpReason getWakeUpReason()
{
    return cachedWakeUpReason;
}

static unsigned long calcSleepTime(SupervisorParameters *p)
{
    unsigned int secondsToNextWakeUp = (p->manager->getNextWakeUpPeriod)() / 1000; // todo fix it by decision what it should be - secs or millis
    if (secondsToNextWakeUp == NW_NO_SLEEP)
    {
        return NW_NO_SLEEP;
    }
    Date now = (p->rtcApi->getDate)();
    unsigned int secondsToNextHour = secondsToNextHourStart(now);
    unsigned int minimum = secondsToNextHour;
    if (secondsToNextWakeUp < secondsToNextHour)
    {
        minimum = secondsToNextWakeUp;
    }
    return minimum;
}

static void tryToSleep(SupervisorParameters *p, unsigned long sleepTime)
{
    if (sleepTime == NW_NO_SLEEP)
    {
        (p->systemApi->log)(SUPERVISOR, "sleep prevented by application");
    }
    else
    {
        sleepTime = sleepTime - SLEEP_TIME_TRESHOLD; // todo consinder the new parameter
        if (sleepTime > 1)
        {
            cachedWakeUpReason = (p->supervisorSleep)(p, sleepTime);
        }
    }
    *p->lastUserEventTimestamp = (p->systemApi->time)();
}

static bool timeToSleep(SupervisorParameters *p)
{
    long lastEventTimestamp = *p->lastUserEventTimestamp;
    long current = (p->systemApi->time)();
    long diff = current - lastEventTimestamp;
    cachedTimeToSleep = p->goToSleepTime - diff;
    (p->systemApi->log)(SUPERVISOR, "diff %d", diff);
    return diff >= p->goToSleepTime;
}

WakeUpReason supervisorSleep(void *v, unsigned int sleepTimeSec)
{
    SupervisorParameters *p = (SupervisorParameters *)v;
    (p->systemApi->log)(SUPERVISOR, "sleep for %d sec", sleepTimeSec);
    (p->watchApi->beforeGoToSleep)();
    WakeUpReason wakeUpReason = (p->watchApi->goToSleep)(sleepTimeSec * uS_TO_S_FACTOR); // here it stops
    (p->watchApi->afterWakeUp)();
    (p->systemApi->log)(SUPERVISOR, "after wake up");
    return wakeUpReason;
}

void supervisor(SupervisorParameters *p)
{
    if ((p->systemApi->take)(p->watchMutex, 10)) // todo there was missprint lastEventTimestamp vs lastEventTimestampMutex - tests dont see it
    {
        cachedNextWakeUpPeriod = calcSleepTime(p);
        if (timeToSleep(p))
        {
            tryToSleep(p, cachedNextWakeUpPeriod);
        }
        (p->systemApi->give)(p->watchMutex);
    }
    else
    {
        (p->systemApi->log)(SUPERVISOR, "failed to take watch mutex");
    }
}

SupervisorApi watchSupervisorApi()
{
    return {
        .getNextWakeUpPeriod = getNextWakeUpPeriod,
        .getTimeToSleep = getTimeToSleep,
        .getWakeUpReason = getWakeUpReason,
    };
}