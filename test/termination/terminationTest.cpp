#include <unity.h>
#include "supervisor.cpp"

int terminationMutex1;
int terminationMutex2;
int takeCount;
int giveCount;

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

bool takeFail(void *semaphore, unsigned int blockTime)
{
    takeCount++;
    return false;
}

void setUp(void)
{
    takeCount = 0;
    giveCount = 0;
    p1 = {
        .terminationMutex = &terminationMutex1,
        .termination = false,
        .take = take,
        .give = give
    }; 
    p2 = {
        .terminationMutex = &terminationMutex2,
        .termination = false,
        .take = take,
        .give = give
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

void whenCouldntTakeMutex()
{
    TaskParameters *tasks[] = {&p1, &p2};
    bool result = setTermination(tasks, 2, 2); 

    TEST_ASSERT_EQUAL_UINT8(1, result); // THEN returns true
    TEST_ASSERT_EQUAL_UINT8(1, p1.termination); // THEN first task has termination flag set
    TEST_ASSERT_EQUAL_UINT8(1, p2.termination); // THEN second task has termination flag set
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenHappyFlow);
    UNITY_END();
}