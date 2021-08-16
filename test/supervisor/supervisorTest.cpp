#include <unity.h>

#include "../systemMock.hpp"
#include "../watchMock.hpp"
#include "../rtcMock.hpp"

#include "supervisor/supervisor.cpp"

long timeResult;
Date dateResult;
bool goToSleepCalled;
unsigned short sleepTimeSecValue;
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

void supervisorSleepMock(void *p, unsigned short sleepTimeSec)
{
    goToSleepCalled = true;
    sleepTimeSecValue = sleepTimeSec;
}

void supervisorSleepSomeTimeMock(void *p, unsigned short sleepTimeSec)
{
    goToSleepCalled = true;
    sleepTimeSecValue = sleepTimeSec;
    timeResult = 48; // to simulate time pass
}

Date getDateStub()
{
    return dateResult;
}

void setUp(void)
{
    timeResult = 0;
    goToSleepCalled = false;
    sleepTimeSecValue = -1;
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
        .supervisorSleep = supervisorSleepMock,
        .systemApi = &systemApi,
        .watchApi = &watchApi,
        .rtcApi = &rtcApi,
    };
}

void whenActionModeAndIdleTimePassed()
{
    rtcApi.getDate = getDateStub;

    dateResult = {
        .year = 2021,
        .month = 8,
        .day = 13,
        .hour = 7,
        .minute = 0,
        .second = 1, // start of the the new hour 
    };
    
    lastUserEventTimestamp = 1;
    timeResult = 15;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(1, goToSleepCalled); // THEN go to sleep
}

void whenAfterWakeUp()
{
    lastUserEventTimestamp = 1;
    timeResult = 15;
    p.supervisorSleep = supervisorSleepSomeTimeMock;

    supervisor(&p);

    TEST_ASSERT_EQUAL_INT64(48, lastUserEventTimestamp); // THEN go to sleep
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
    rtcApi.getDate = getDateStub;

    dateResult = {
        .year = 2021,
        .month = 8,
        .day = 13,
        .hour = 7,
        .minute = 00,
        .second = 00,
    };
    unsigned short sleepTime = calcSleepTime(&p);
    TEST_ASSERT_EQUAL_UINT16(0, sleepTime);

    dateResult.minute = 0;
    dateResult.second = 1;
    sleepTime = calcSleepTime(&p);
    TEST_ASSERT_EQUAL_UINT16(59 * 60 + 59, sleepTime);

    dateResult.minute = 0;
    dateResult.second = 59;
    sleepTime = calcSleepTime(&p);
    TEST_ASSERT_EQUAL_UINT16(59 * 60 + 1, sleepTime);

    dateResult.minute = 1;
    dateResult.second = 0;
    sleepTime = calcSleepTime(&p);
    TEST_ASSERT_EQUAL_UINT16(58 * 60 + 60, sleepTime);

    dateResult.minute = 59;
    dateResult.second = 0;
    sleepTime = calcSleepTime(&p);
    TEST_ASSERT_EQUAL_UINT16(0 * 60 + 60, sleepTime);

    dateResult.minute = 59;
    dateResult.second = 59;
    sleepTime = calcSleepTime(&p);
    TEST_ASSERT_EQUAL_UINT16(0 * 60 + 1, sleepTime);

    dateResult.minute = 59;
    dateResult.second = 58;
    sleepTime = calcSleepTime(&p);
    TEST_ASSERT_EQUAL_UINT16(0 * 60 + 2, sleepTime);
}

void whenSleepTimeLesserThanGotoSleepPeriod()
{
    rtcApi.getDate = getDateStub;

    dateResult = {
        .year = 2021,
        .month = 8,
        .day = 13,
        .hour = 7,
        .minute = 59,
        .second = 57, // three seconds to the new hour
    };
    lastUserEventTimestamp = 1;
    timeResult = 15;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(0, goToSleepCalled); // THEN no sleep
}

void whenSleepTimeGreaterThanGotoSleepPeriod()
{
    rtcApi.getDate = getDateStub;

    dateResult = {
        .year = 2021,
        .month = 8,
        .day = 13,
        .hour = 7,
        .minute = 59,
        .second = 50, // ten seconds to the new hour 
    };
    lastUserEventTimestamp = 1;
    timeResult = 15;

    supervisor(&p);

    TEST_ASSERT_EQUAL_UINT8(1, goToSleepCalled); // THEN sleep
    TEST_ASSERT_EQUAL_UINT8(7, sleepTimeSecValue); // THEN for 7 second
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenActionModeAndIdleTimePassed);
    RUN_TEST(whenAfterWakeUp);
    RUN_TEST(whenActionModeAndIdleTimeNotPassed);
    RUN_TEST(whenSleepModeAndEvent);
    RUN_TEST(calcSleepTimeTests);
    RUN_TEST(whenSleepTimeLesserThanGotoSleepPeriod);
    RUN_TEST(whenSleepTimeGreaterThanGotoSleepPeriod);
    UNITY_END();
}