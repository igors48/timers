#include <unity.h>
#include "task.cpp"

/*
typedef struct
{
    Func func;
    void *parameters;
    void *mutex;
    bool termination;
    bool canBeSuspended;
    unsigned int taskDelay;
    Take take;
    Give give;
    Delay delay;
    Log log;
} TaskParameters;
*/

bool giveResult;
bool takeResult;
bool funcCalled;

int mutex;
bool termination;
bool canBeSuspended;

TaskParameters parameters;

void func(void *p)
{
    funcCalled = true;
}

bool give(void *semaphore)
{
    return giveResult;
}

bool take(void *semaphore, unsigned int blockTime)
{
    return takeResult;
}

void setUp(void)
{
    giveResult = true;
    takeResult = true;
    funcCalled = false;
    termination = false;
    canBeSuspended = false;

    parameters = {
    .func = func,
    .parameters = NULL,
    .mutex = &mutex,
    .termination = termination,
    .canBeSuspended = canBeSuspended,
    .taskDelay = 0,
    .take = take,
    .give = give,
    .delay = NULL,
    .log = log
    };
}

int main()
{
    UNITY_BEGIN();
    //RUN_TEST(whenTimeSinceLastTouchLesserThan5Sec);
    UNITY_END();
}