#include "supervisor.hpp"

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

void goToSleep(void *v)
{
    SupervisorParameters *p = (SupervisorParameters *)v;
    suspendTasks(p->tasks, p->tasksCount, p->systemApi->suspend);
    p->watchApi->beforeGoToSleep();
    p->watchApi->goToSleep(); // here it stops
    p->watchApi->afterWakeUp();
    p->systemApi->log(SUPERVISOR, "after wake up");
    resumeTasks(p->tasks, p->tasksCount, p->systemApi->resume);
}

unsigned short calcSleepTime(SupervisorParameters *p)
{
    Date now = p->rtcApi->getDate();
    unsigned char seconds = now.second;
    unsigned char minutes = now.minute;
    seconds = (seconds == 0) ? 0 : 59 - seconds;
    minutes = (minutes == 0) ? 0 : 59 - minutes;
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
        bool sleep = diff >= p->goToSleepTime;
        if (sleep)
        {
            unsigned short sleepTime = calcSleepTime(p);
            if (sleepTime > p->goToSleepTime)
            {
                p->goToSleep(p);
            }
            *p->lastUserEventTimestamp = p->systemApi->time();
        }
        p->systemApi->give(p->watchMutex);
    }
    else
    {
        p->systemApi->log(SUPERVISOR, "failed to take last event timestamp mutex");
    }
}
