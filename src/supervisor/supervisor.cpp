#include "supervisor.hpp"

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define SLEEP_TIME_TRESHOLD 3 

static const char SUPERVISOR[] = "supervisor";

void suspendTasks(void **tasks, int count, Suspend suspend)
{
    for (int i = 0; i < count; i++)
    {
        void *current = tasks[i];
        suspend(current);
    }
}

void resumeTasks(void **tasks, int count, Resume resume)
{
    for (int i = 0; i < count; i++)
    {
        void *current = tasks[i];
        resume(current);
    }
}

void supervisorSleep(void *v, unsigned short sleepTimeSec)
{
    SupervisorParameters *p = (SupervisorParameters *)v;
    suspendTasks(p->tasks, p->tasksCount, p->systemApi->suspend); // todo seems not needed
    p->systemApi->log(SUPERVISOR, "sleep for %d sec", sleepTimeSec);
    p->watchApi->beforeGoToSleep();
    p->watchApi->goToSleep(sleepTimeSec * uS_TO_S_FACTOR); // here it stops
    p->watchApi->afterWakeUp();
    p->systemApi->log(SUPERVISOR, "after wake up");
    resumeTasks(p->tasks, p->tasksCount, p->systemApi->resume); // todo seems not needed
}

unsigned short calcSleepTime(SupervisorParameters *p)
{
    Date now = p->rtcApi->getDate();
    unsigned char seconds = now.second;
    unsigned char minutes = now.minute;
    if (seconds == 0 && minutes == 0) {
        return 0;
    }
    seconds = 60 - seconds;
    minutes = 59 - minutes;
    return minutes * 60 + seconds;
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
            short sleepTime = calcSleepTime(p);
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
        p->systemApi->log(SUPERVISOR, "failed to take last event timestamp mutex"); // todo fix log message -> watch mutex
    }
}
