#include <unity.h>

#include "../systemMock.hpp"

#include "supervisor/supervisor.cpp"

long timeResult;
bool goToSleepCalled;
bool wakeUpCalled;
bool action;
long lastEventTimestamp;
long wakeUpTime;
long goToSleepTime;

int actionMutex;
int lastEventTimestampMutex;

SystemApi systemApi;

SupervisorParameters p;

/*
bool give(void *semaphore)
{
    return true;
}

bool take(void *semaphore, unsigned int blockTime)
{
    return true;
}
*/
long time()
{
    return timeResult;
}
/*
void log(const char *source, const char *message, ...)
{
}
*/
void wakeUp(void *p)
{
    wakeUpCalled = true;
}

void goToSleep(void *p)
{
    goToSleepCalled = true;
}

void setUp(void)
{
    timeResult = 0;
    goToSleepCalled = false;
    wakeUpCalled = false;
    action = false;
    lastEventTimestamp = 0;
    wakeUpTime = 1;
    goToSleepTime = 10;

    systemApi = systemApiMock();

    p = {
        .actionMutex = &actionMutex,
        .action = &action,
        .lastEventTimestampMutex = &lastEventTimestampMutex,
        .lastEventTimestamp = &lastEventTimestamp,
        .wakeUpTime = wakeUpTime,
        .wakeUp = wakeUp,
        .goToSleepTime = goToSleepTime,
        .goToSleep = goToSleep,
        .systemApi = &systemApi};
}

void whenActionModeAndIdleTimePassed()
{
    action = true;
    lastEventTimestamp = 1;
    timeResult = 15;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(1, goToSleepCalled); // THEN go to sleep
    TEST_ASSERT_EQUAL_UINT8(0, wakeUpCalled);
}

void whenActionModeAndIdleTimeNotPassed()
{
    action = true;
    lastEventTimestamp = 1;
    timeResult = 5;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(0, goToSleepCalled); // THEN stay in action
    TEST_ASSERT_EQUAL_UINT8(0, wakeUpCalled);
}

void whenSleepModeAndNoEvent()
{
    action = false;
    lastEventTimestamp = 1;
    timeResult = 15;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(0, goToSleepCalled); // THEN stay sleep
    TEST_ASSERT_EQUAL_UINT8(0, wakeUpCalled);
}

void whenSleepModeAndEvent()
{
    action = false;
    lastEventTimestamp = 1;
    timeResult = 2;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(0, goToSleepCalled); // THEN wake up
    TEST_ASSERT_EQUAL_UINT8(1, wakeUpCalled);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenActionModeAndIdleTimePassed);
    RUN_TEST(whenActionModeAndIdleTimeNotPassed);
    RUN_TEST(whenSleepModeAndNoEvent);
    RUN_TEST(whenSleepModeAndEvent);
    UNITY_END();
}