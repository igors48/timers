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

bool setTermination(TaskParameters *tasks[], int count)
{
    bool notDone = true;
    int triesLeft = 10;
    while (notDone && triesLeft > 0)
    {
        notDone = false;
        for (int i = 0; i < count; i++)
        {
            auto current = tasks[i];
            auto mutex = current->terminationMutex;
            if (current->take(mutex, 1))
            {
                current->termination = true;
                current->give(mutex);
            }
            else
            {
                current->log("", "");
                notDone = true;
            }
        }
        triesLeft--;
    }
    return !notDone;
}

bool waitForSuspend(TaskParameters *tasks[], int count)
{
    bool notDone = true;
    int triesLeft = 10;
    while (notDone && triesLeft > 0)
    {
        notDone = false;
        for (int i = 0; i < count; i++)
        {
            auto current = tasks[i];
            auto mutex = current->terminationMutex;
            if (current->take(mutex, 1))
            {
                bool canBeSuspended = current->canBeSuspended;
                current->give(mutex);
                if (!canBeSuspended)
                {
                    current->log("", "");
                    notDone = true;
                }
            }
            else
            {
                current->log("", "");
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
        suspend(tasks[i]);
    }
}