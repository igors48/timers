#include "task.hpp"

void _actionMode(TaskParameters *p)
{
    if (p->systemApi->take(p->actionMutex, 0))
    {
        bool action = *p->action;
        p->systemApi->give(p->actionMutex);
        if (action)
        {
            p->func(p->parameters);
        }
    }
}

void _sleepMode(TaskParameters *p)
{
    if (p->systemApi->take(p->actionMutex, 0))
    {
        bool sleep = !(*p->action);
        p->systemApi->give(p->actionMutex);
        if (sleep)
        {
            p->func(p->parameters);
        }
    }
}

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

void _actionModeWrapper(TaskParameters *p)
{
    if (p->systemApi->take(p->terminationMutex, 0))
    {
        bool termination = p->termination;
        p->canBeSuspended = termination;
        p->systemApi->give(p->terminationMutex);
        if (!termination)
        {
            _actionMode(p);
        }
    }
}

void _sleepModeWrapper(TaskParameters *p)
{
    if (p->systemApi->take(p->terminationMutex, 0))
    {
        bool termination = p->termination;
        p->canBeSuspended = termination;
        p->systemApi->give(p->terminationMutex);
        if (!termination)
        {
            _sleepMode(p);
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

void actionModeTask(void *p)
{
    TaskParameters *t = (TaskParameters *)p;
    while (true)
    {
        _actionModeWrapper(t);
        t->systemApi->delay(t->taskDelay);
    }
}

void sleepModeTask(void *p)
{
    TaskParameters *t = (TaskParameters *)p;
    while (true)
    {
        _sleepModeWrapper(t);
        t->systemApi->delay(t->taskDelay);
    }
}
/*
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
*/