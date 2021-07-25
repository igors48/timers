#include <unity.h>

#include "../systemMock.hpp"
#include "../watchMock.hpp"

#include "supervisor/supervisor.cpp"

long timeResult;
bool goToSleepCalled;
long lastEventTimestamp;
long goToSleepTime;

int lastEventTimestampMutex;

SystemApi systemApi;
WatchApi watchApi;

SupervisorParameters p;

long time() // todo name convention for common and local mock funcs
{
    return timeResult;
}

void goToSleepMock(void *p)
{
    goToSleepCalled = true;
}

void setUp(void)
{
    timeResult = 0;
    goToSleepCalled = false;
    lastEventTimestamp = 0;
    goToSleepTime = 10;

    systemApi = systemApiMock();
    systemApi.time = time;

    watchApi = watchApiMock();

    p = {
        .lastEventTimestampMutex = &lastEventTimestampMutex,
        .lastEventTimestamp = &lastEventTimestamp,
        .goToSleepTime = goToSleepTime,
        .goToSleep = goToSleepMock,
        .systemApi = &systemApi,
        .watchApi = &watchApi};
}

void whenActionModeAndIdleTimePassed()
{
    lastEventTimestamp = 1;
    timeResult = 15;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(1, goToSleepCalled); // THEN go to sleep
}

void whenActionModeAndIdleTimeNotPassed()
{
    lastEventTimestamp = 1;
    timeResult = 5;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(0, goToSleepCalled); // THEN stay in action
}

void whenSleepModeAndEvent()
{
    lastEventTimestamp = 1;
    timeResult = 2;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(0, goToSleepCalled); // THEN wake up
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenActionModeAndIdleTimePassed);
    RUN_TEST(whenActionModeAndIdleTimeNotPassed);
    RUN_TEST(whenSleepModeAndEvent);
    UNITY_END();
}