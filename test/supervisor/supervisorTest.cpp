#include <unity.h>

#include "../systemMock.hpp"
#include "../watchMock.hpp"
#include "../rtcMock.hpp"

#include "supervisor/supervisor.cpp"

long timeResult;
bool goToSleepCalled;
long lastUserEventTimestamp;
long goToSleepTime;

int watchMutex;

SystemApi systemApi;
WatchApi watchApi;
RtcApi rtcApi;

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
    lastUserEventTimestamp = 0;
    goToSleepTime = 10;

    systemApi = systemApiMock();
    systemApi.time = time;

    watchApi = watchApiMock();
    rtcApi = rtcApiMock();

    p = {
        .watchMutex = &watchMutex,
        .lastUserEventTimestamp = &lastUserEventTimestamp,
        .goToSleepTime = goToSleepTime,
        .goToSleep = goToSleepMock,
        .systemApi = &systemApi,
        .watchApi = &watchApi,
        .rtcApi = &rtcApi,
        };
}

void whenActionModeAndIdleTimePassed()
{
    lastUserEventTimestamp = 1;
    timeResult = 15;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(1, goToSleepCalled); // THEN go to sleep
}

void whenActionModeAndIdleTimeNotPassed()
{
    lastUserEventTimestamp = 1;
    timeResult = 5;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(0, goToSleepCalled); // THEN stay in action
}

void whenSleepModeAndEvent()
{
    lastUserEventTimestamp = 1;
    timeResult = 2;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(0, goToSleepCalled); // THEN wake up
}

void calcSleepTimeTests()
{
    unsigned short sleepTime = calcSleepTime(&p);
    TEST_ASSERT_EQUAL_UINT16(0, sleepTime); 
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenActionModeAndIdleTimePassed);
    RUN_TEST(whenActionModeAndIdleTimeNotPassed);
    RUN_TEST(whenSleepModeAndEvent);
    RUN_TEST(calcSleepTimeTests);
    UNITY_END();
}