#include <unity.h>

#include "../systemMock.hpp"

#include "supervisor/supervisor.cpp"

int terminationMutex1;
int terminationMutex2;
int takeCount;
int giveCount;
bool takeResult;

SystemApi systemApi;

TaskParameters p1;
TaskParameters p2;

bool give(void *semaphore)
{
    giveCount++;
    return true;
}

bool take(void *semaphore, unsigned int blockTime)
{
    takeCount++;
    return true;
}

bool takeBlink(void *semaphore, unsigned int blockTime)
{
    takeCount++;
    bool result = takeResult;
    takeResult = !takeResult;
    return result;
}

void setUp(void)
{
    takeCount = 0;
    giveCount = 0;
    takeResult = true;

    systemApi = systemApiMock();
    systemApi.take = take;
    systemApi.give = give;

    p1 = {
        .terminationMutex = &terminationMutex1,
        .termination = false,
        .systemApi = &systemApi
    }; 
    p2 = {
        .terminationMutex = &terminationMutex2,
        .termination = false,
        .systemApi = &systemApi
    }; 
}

void whenHappyFlow()
{
    TaskParameters *tasks[] = {&p1, &p2};
    bool result = setTermination(tasks, 2, 2); 

    TEST_ASSERT_EQUAL_UINT8(1, result); // THEN returns true
    TEST_ASSERT_EQUAL_UINT8(1, p1.termination); // THEN first task has termination flag set
    TEST_ASSERT_EQUAL_UINT8(1, p2.termination); // THEN second task has termination flag set
    TEST_ASSERT_EQUAL_UINT8(2, takeCount); // THEN only one take for each task
    TEST_ASSERT_EQUAL_UINT8(2, giveCount); // THEN only one give for each task
}

void whenCouldntTakeMutexOnSecondTask()
{
    TaskParameters *tasks[] = {&p1, &p2};
    p2.systemApi->take = takeBlink;
    bool result = setTermination(tasks, 2, 2); 

    TEST_ASSERT_EQUAL_UINT8(0, result); // THEN returns false
    TEST_ASSERT_EQUAL_UINT8(1, p1.termination); // THEN first task has termination flag set
    TEST_ASSERT_EQUAL_UINT8(0, p2.termination); // THEN second task has termination flag not set
    TEST_ASSERT_EQUAL_UINT8(4, takeCount); // THEN +2 additional failed takes compare to happy flow
    TEST_ASSERT_EQUAL_UINT8(2, giveCount); // THEN only one give for p1 task
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenHappyFlow);
    RUN_TEST(whenCouldntTakeMutexOnSecondTask);
    UNITY_END();
}