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
        .canBeSuspended = true,
        .take = take,
        .give = give
    }; 
    p2 = {
        .terminationMutex = &terminationMutex2,
        .canBeSuspended = true,
        .take = take,
        .give = give
    }; 
}

void whenHappyFlow()
{
    TaskParameters *tasks[] = {&p1, &p2};
    
    bool result = waitForSuspend(tasks, 2, 2); 

    TEST_ASSERT_EQUAL_UINT8(1, result); // THEN returns true
    TEST_ASSERT_EQUAL_UINT8(2, takeCount); // THEN only one take for each task
    TEST_ASSERT_EQUAL_UINT8(2, giveCount); // THEN only one give for each task
}

void whenOneTaskCanNotBeSuspended()
{
    TaskParameters *tasks[] = {&p1, &p2};
    p1.canBeSuspended = false;

    bool result = waitForSuspend(tasks, 2, 2); 

    TEST_ASSERT_EQUAL_UINT8(0, result); // THEN returns true
    TEST_ASSERT_EQUAL_UINT8(4, takeCount); // THEN 2 tries performed for each task
    TEST_ASSERT_EQUAL_UINT8(4, giveCount); // THEN 2 gives performed for each task
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenHappyFlow);
    UNITY_END();
}