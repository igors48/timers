#include "supervisor.hpp"

static const char SUPERVISOR[] = "supervisor";

void supervisor(SupervisorParameters *p)
{
    if (p->systemApi->take(p->actionMutex, 10))
    {
        bool action = *p->action;
        p->systemApi->give(p->actionMutex);
        p->systemApi->log(SUPERVISOR, "action mode %d", action);
        if (p->systemApi->take(p->lastEventTimestampMutex, 10)) // todo there was missprint lastEventTimestamp vs lastEventTimestampMutex - tests dont see it
        {
            long lastEventTimestamp = *p->lastEventTimestamp;
            p->systemApi->give(p->lastEventTimestampMutex);
            long current = p->systemApi->time();
            long diff = current - lastEventTimestamp;
            p->systemApi->log(SUPERVISOR, "diff %d", diff);
            bool sleep = action && (diff >= p->goToSleepTime);
            if (sleep)
            {
                p->systemApi->log(SUPERVISOR, "before go to sleep");
                p->goToSleep(p);
            }
        }
        else
        {
            p->systemApi->log(SUPERVISOR, "failed to take last event timestamp mutex");
        }
    }
    else
    {
        p->systemApi->log(SUPERVISOR, "failed to take action mutex");
    }
}

bool setTermination(TaskParameters **tasks, int count, int tryCount, bool value)
{
    bool notDone = true;
    int triesLeft = tryCount;
    while (notDone && triesLeft > 0)
    {        
        notDone = false;
        for (int i = 0; i < count; i++)
        {
            TaskParameters *current = tasks[i];
            
            void *mutex = current->terminationMutex;
            if (current->systemApi->take(mutex, 1))
            {
                current->termination = value;
                current->systemApi->give(mutex);
            }
            else
            {
                notDone = true;
            }
        }
        triesLeft--;
    }
    return !notDone;
}

bool waitForSuspend(TaskParameters **tasks, int count, int tryCount)
{
    bool notDone = true;
    int triesLeft = tryCount;
    while (notDone && triesLeft > 0)
    {
        notDone = false;
        for (int i = 0; i < count; i++)
        {
            TaskParameters *current = tasks[i];
            void *mutex = current->terminationMutex;
            if (current->systemApi->take(mutex, 1))
            {
                bool canBeSuspended = current->canBeSuspended;
                current->systemApi->give(mutex);
                if (!canBeSuspended)
                {
                    notDone = true;
                }
            }
            else
            {
                notDone = true;
            }
        }
        triesLeft--;
    }
    return !notDone;
}

void suspendTasks(TaskParameters **tasks, int count, Suspend suspend)
{
    for (int i = 0; i < count; i++)
    {
        suspend(tasks[i]->handle);
    }
}

void resumeTasks(TaskParameters **tasks, int count, Resume resume)
{
    for (int i = 0; i < count; i++)
    {
        resume(tasks[i]->handle);
    }
}

void goToSleep(void *v)
{
    SupervisorParameters *p = (SupervisorParameters *)v;
    p->systemApi->log(SUPERVISOR, "go to sleep");
    if (p->systemApi->take(p->actionMutex, 10))
    {
        *p->action = false; // todo cover by test 
        p->systemApi->give(p->actionMutex);

        setTermination(p->tasks, p->tasksCount, 3, true);
        waitForSuspend(p->tasks, p->tasksCount, 3);
        suspendTasks(p->tasks, p->tasksCount, p->systemApi->suspend);
    }
}
