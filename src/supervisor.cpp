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