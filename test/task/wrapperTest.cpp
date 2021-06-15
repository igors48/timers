#include <unity.h>
#include "task.cpp"

bool giveResult;
bool takeResult;
bool funcCalled;

int terminationMutex;
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

void log(const char *source, const char *message, ...)
{
}

void frDelay(unsigned int time)
{
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
    .terminationMutex = &terminationMutex,
    .termination = termination,
    .canBeSuspended = canBeSuspended,
    .taskDelay = 0,
    .take = take,
    .give = give,
    .delay = frDelay,
    .log = log
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