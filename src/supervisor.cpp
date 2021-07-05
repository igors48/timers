#include <supervisor.hpp>

static const char SUPERVISOR[] = "supervisor";

void supervisor(SupervisorParameters *p)
{
    if (p->take(p->actionMutex, 10))
    {
        bool action = *p->action;
        p->give(p->actionMutex);
        if (p->take(p->lastEventTimestamp, 10))
        {
            long lastEventTimestamp = *p->lastEventTimestamp;
            p->give(p->lastEventTimestamp);
            long current = p->time();
            long diff = current - lastEventTimestamp;
            bool sleep = action && (diff >= p->goToSleepTime);
            if (sleep)
            {
                p->goToSleep(p);
            }
            else
            {
                bool wakeUp = !action && (diff <= p->wakeUpTime);
                if (wakeUp)
                {
                    p->wakeUp(p);
                }
            }
        }
        else
        {
            p->log(SUPERVISOR, "failed to take last event timestamp mutex");
        }
    }
    else
    {
        p->log(SUPERVISOR, "failed to take action mutex");
    }
}

bool setTermination(TaskParameters *tasks[], int count, int tryCount)
{
    bool notDone = true;
    int triesLeft = tryCount;
    while (notDone && triesLeft > 0)
    {
        notDone = false;
        for (int i = 0; i < count; i++)
        {
            TaskParameters* current = tasks[i];
            void* mutex = current->terminationMutex;
            if (current->take(mutex, 1))
            {
                current->termination = true;
                current->give(mutex);
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

bool waitForSuspend(TaskParameters *tasks[], int count, int tryCount)
{
    bool notDone = true;
    int triesLeft = tryCount;
    while (notDone && triesLeft > 0)
    {
        notDone = false;
        for (int i = 0; i < count; i++)
        {
            TaskParameters* current = tasks[i];
            void* mutex = current->terminationMutex;
            if (current->take(mutex, 1))
            {
                bool canBeSuspended = current->canBeSuspended;
                current->give(mutex);
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

void suspendTasks(TaskParameters *tasks[], int count, Suspend suspend)
{
    for (int i = 0; i < count; i++)
    {
        suspend(tasks[i]->handle);
    }
}

void resumeTasks(TaskParameters *tasks[], int count, Resume resume)
{
    for (int i = 0; i < count; i++)
    {
        resume(tasks[i]->handle);
    }
}

void goToSleep(SupervisorParameters *p)
{
    if (p->take(p->actionMutex, 10))
    {
        *p->action = false;
        p->give(p->actionMutex);

        setTermination(p->actionModeTasks, p->actionModeTasksCount, 3);
        waitForSuspend(p->actionModeTasks, p->actionModeTasksCount, 3);
        suspendTasks(p->actionModeTasks, p->actionModeTasksCount, p->suspend);

        resumeTasks(p->sleepModeTasks, p->sleepModeTasksCount, p->resume);
    }
}

void wakeUp(SupervisorParameters *p)
{
    if (p->take(p->actionMutex, 10))
    {
        *p->action = true;
        p->give(p->actionMutex);

        setTermination(p->sleepModeTasks, p->sleepModeTasksCount, 3);
        waitForSuspend(p->sleepModeTasks, p->sleepModeTasksCount, 3);
        suspendTasks(p->sleepModeTasks, p->sleepModeTasksCount, p->suspend);

        resumeTasks(p->actionModeTasks, p->actionModeTasksCount, p->resume);
    }
}