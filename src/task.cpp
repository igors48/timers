#include "task.hpp"

void wrapper(TaskParameters *p)
{
    if (p->take(p->terminationMutex, 0))
    {
        bool termination = p->termination;
        p->canBeSuspended = termination;
        p->give(p->terminationMutex);
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
        wrapper(t);
        t->delay(t->taskDelay);
    }
}

TaskParameters create(Func func, void *parameters, void *terminationMutex, unsigned int taskDelay)
{
    TaskParameters result = {
        .func = func,
        .parameters = parameters,
        .terminationMutex = terminationMutex,
        .termination = false,
        .canBeSuspended = false,
        .taskDelay = taskDelay,
        .take = take,
        .give = give,
        .delay = frDelay,
        .log = log
    };

    return result;
}