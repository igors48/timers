#include <unity.h>

#include "../systemMock.hpp"

#include "supervisor/task.cpp"

bool funcCalled;
int terminationMutex;
bool termination;
bool canBeSuspended;

SystemApi systemApi;

TaskParameters parameters;

void func(void *p)
{
    funcCalled = true;
}

void setUp(void)
{
    funcCalled = false;
    termination = false;
    canBeSuspended = false;

    systemApi = systemApiMock();

    parameters = {
        .handle = NULL,
        .func = func,
        .parameters = NULL,
        .terminationMutex = &terminationMutex,
        .termination = termination,
        .canBeSuspended = canBeSuspended,
        .taskDelay = 0,
        .systemApi = &systemApi};
}

void whenTerminationFlagIsTrue()
{
    parameters.canBeSuspended = false;
    parameters.termination = true;
    _wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, parameters.canBeSuspended); // THEN canBeSuspended set to true
    TEST_ASSERT_EQUAL_INT(0, funcCalled);                // THEN task function did not called
}

void whenTerminationFlagIsFalse()
{
    funcCalled = false;
    _wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(0, parameters.canBeSuspended); // THEN canBeSuspended still set to false
    TEST_ASSERT_EQUAL_INT(1, funcCalled);                // THEN task function called
}

void whenAction()
{
    funcCalled = false;
    _wrapper(&parameters);
    TEST_ASSERT_EQUAL_INT(1, funcCalled); // THEN common task function called
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTerminationFlagIsTrue);
    RUN_TEST(whenTerminationFlagIsFalse);
    RUN_TEST(whenAction);
    UNITY_END();
}