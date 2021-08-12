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
    *p->lastUserEventTimestamp = p->systemApi->time(); // todo - cover with tests or even move this to the supervisor func
    resumeTasks(p->tasks, p->tasksCount, p->systemApi->resume);
}

unsigned short calcSleepTime(SupervisorParameters *p)
{
    Date now = p->rtcApi->getDate();
    unsigned char seconds = 60 - now.second;
    unsigned char minutes = 60 - now.minute;
    unsigned short diff = minutes * 60 + seconds;
    return diff;
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
            p->goToSleep(p);
        }
        p->systemApi->give(p->watchMutex);
    }
    else
    {
        p->systemApi->log(SUPERVISOR, "failed to take last event timestamp mutex");
    }
}
