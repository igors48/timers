#include "task.hpp"

void _wrapper(TaskParameters *p)
{
    if (p->systemApi->take(p->terminationMutex, 0))
    {
        bool termination = p->termination;
        p->canBeSuspended = termination;
        p->systemApi->give(p->terminationMutex);
        if (!termination)
        {
            p->func(p->parameters);
        }
    }
}

void task(void *p)
{
    TaskParameters *t = (TaskParameters *)p;
    while (true)
    {
        _wrapper(t);
        t->systemApi->delay(t->taskDelay);
    }
}
