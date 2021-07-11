#include <unity.h>
#include "task/task.cpp"

bool giveResult;
bool takeResult;
bool funcCalled;

int terminationMutex;
bool termination;
int actionMutex;
bool action;
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
    action = false;
    termination = false;
    canBeSuspended = false;

    parameters = {
    .func = func,
    .parameters = NULL,
    .actionMutex = &actionMutex,
    .action = &action,
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
    _wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, parameters.canBeSuspended); // THEN canBeSuspended set to true
    TEST_ASSERT_EQUAL_INT(0, funcCalled); // THEN task function did not called

    parameters.canBeSuspended = false;
    _actionModeWrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, parameters.canBeSuspended); // THEN canBeSuspended set to true
    TEST_ASSERT_EQUAL_INT(0, funcCalled); // THEN task function did not called

    parameters.canBeSuspended = false;
    _sleepModeWrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, parameters.canBeSuspended); // THEN canBeSuspended set to true
    TEST_ASSERT_EQUAL_INT(0, funcCalled); // THEN task function did not called
}

void whenTerminationFlagIsFalse() {
    _wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(0, parameters.canBeSuspended); // THEN canBeSuspended still set to false
    TEST_ASSERT_EQUAL_INT(1, funcCalled); // THEN task function called

    funcCalled = false;
    _actionModeWrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(0, parameters.canBeSuspended); // THEN canBeSuspended still set to false
    TEST_ASSERT_EQUAL_INT(0, funcCalled); // THEN task function did not called because action is false

    funcCalled = false;
    _sleepModeWrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(0, parameters.canBeSuspended); // THEN canBeSuspended still set to false
    TEST_ASSERT_EQUAL_INT(1, funcCalled); // THEN task function called because action is false
}

void whenActionMode() {
    action = true;
    _actionModeWrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, funcCalled); // THEN action mode task function called
    
    funcCalled = false;
    _sleepModeWrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(0, funcCalled); // THEN sleep mode task function not called

    funcCalled = false;
    _wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, funcCalled); // THEN common task function called
}

void whenSleepMode() {
    _actionModeWrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(0, funcCalled); // THEN action mode task function not called
    
    _sleepModeWrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, funcCalled); // THEN sleep mode task function called

    funcCalled = false;
    _wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, funcCalled); // THEN common task function called
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTerminationFlagIsTrue);
    RUN_TEST(whenTerminationFlagIsFalse);
    RUN_TEST(whenActionMode);
    RUN_TEST(whenSleepMode);
    UNITY_END();
}