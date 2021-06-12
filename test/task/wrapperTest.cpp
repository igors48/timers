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
    .log = NULL
    };
}

void whenTerminationFlagIsTrue() {
    parameters.termination = true;
    wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, parameters.canBeSuspended); // THEN canBeSuspended set to true
    TEST_ASSERT_EQUAL_INT(0, funcCalled); // THEN task function did not called
}

void whenTerminationFlagIsFalse() {
    wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(0, parameters.canBeSuspended); // THEN canBeSuspended set to false
    TEST_ASSERT_EQUAL_INT(1, funcCalled); // THEN task function called
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTerminationFlagIsTrue);
    RUN_TEST(whenTerminationFlagIsFalse);
    UNITY_END();
}