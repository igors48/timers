#include "supervisor.hpp"
#include "core/tools/tools.hpp"

#define uS_TO_S_FACTOR 1000000 // Conversion factor for micro seconds to seconds
#define SLEEP_TIME_TRESHOLD 3000

static const char SUPERVISOR[] = "supervisor";

static unsigned long cachedNextWakeUpPeriod = 0;
static unsigned long cachedTimeToSleep = 0;
static WakeUpReason cachedWakeUpReason = WUR_UNKNOWN;

static unsigned long getNextWakeUpPeriod()
{
    return cachedNextWakeUpPeriod;
}

static unsigned long getTimeToSleep()
{
    return cachedTimeToSleep;
}

static WakeUpReason getWakeUpReason()
{
    return cachedWakeUpReason;
}

static unsigned long calcSleepTime(SupervisorParameters *p)
{
    const unsigned long millisToNextWakeUp = (p->manager->getNextWakeUpPeriod)();
    if (millisToNextWakeUp == NW_NO_SLEEP)
    {
        return NW_NO_SLEEP;
    }
    const Date now = (p->rtcApi->getDate)();
    const unsigned long millisToNextHour = secondsToNextHourStart(now) * 1000;
    unsigned long minimum = millisToNextHour;
    if (millisToNextWakeUp < millisToNextHour)
    {
        minimum = millisToNextWakeUp;
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
        sleepTime = sleepTime - SLEEP_TIME_TRESHOLD; // todo consider the new parameter
        if (sleepTime > 1000)
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

WakeUpReason supervisorSleep(void *v, unsigned long sleepTimeMillis)
{
    SupervisorParameters *p = (SupervisorParameters *)v;
    (p->systemApi->log)(SUPERVISOR, "sleep for %d millis", sleepTimeMillis);
    (p->watchApi->beforeGoToSleep)();
    WakeUpReason wakeUpReason = (p->watchApi->goToSleep)(sleepTimeMillis * 1000/*uS_TO_S_FACTOR*/); // here it stops
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