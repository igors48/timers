#include "task.hpp"

void wrapper(TaskParameters *p)
{
    if (p->take(p->mutex, 0))
    {
        bool termination = p->termination;
        p->canBeSuspended = termination;
        p->give(p->mutex);
        if (!termination)
        {
            p->func(p->parameters);
        }
    }
}

void task(TaskParameters *p)
{
    while (true)
    {
        wrapper(p);
        p->delay(p->taskDelay);
    }
}
