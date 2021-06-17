#include "task.hpp"

void wrapper(TaskParameters *p)
{
    if (p->take(p->terminationMutex, 0))
    {
        bool termination = *p->termination;
        p->canBeSuspended = termination;
        p->give(p->terminationMutex);
        if (!termination)
        {
            p->func(p->parameters);
        }
    }
}

void actionModeWrapper(TaskParameters *p)
{
    if (p->take(p->actionMutex, 0))
    {
        bool action = *p->action;
        p->give(p->actionMutex);
        if (action)
        {
            wrapper(p);
        }
    }
}

void sleepModeWrapper(TaskParameters *p)
{
    if (p->take(p->actionMutex, 0))
    {
        bool sleep = !(*p->action);
        p->give(p->actionMutex);
        if (sleep)
        {
            wrapper(p);
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

void actionModeTask(void *p)
{
    TaskParameters *t = (TaskParameters *)p;
    while (true)
    {
        actionModeWrapper(t);
        t->delay(t->taskDelay);
    }
}

void sleepModeTask(void *p)
{
    TaskParameters *t = (TaskParameters *)p;
    while (true)
    {
        sleepModeWrapper(t);
        t->delay(t->taskDelay);
    }
}

TaskParameters create(
    Func func,
    void *parameters,
    void *actionMutex,
    bool *action,
    void *terminationMutex,
    bool *termination,
    unsigned int taskDelay)
{
    TaskParameters result = {
        .func = func,
        .parameters = parameters,
        .actionMutex = actionMutex,
        .action = action,
        .terminationMutex = terminationMutex,
        .termination = termination,
        .canBeSuspended = false,
        .taskDelay = taskDelay,
        .take = take,
        .give = give,
        .delay = frDelay,
        .log = log};

    return result;
}