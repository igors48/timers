#include <unity.h>

#include <cstdio>

#include "../systemMock.hpp"
#include "../watchMock.hpp"
#include "../rtcMock.hpp"

#include "core/supervisor/supervisor.cpp"
#include "core/tools/tools.cpp"

long timeResult;
Date dateResult;
bool goToSleepCalled;
unsigned int sleepTimeSecValue;
long lastUserEventTimestamp;
long goToSleepTime;
unsigned int nextWakeUpPeriod;

int watchMutex;

SystemApi systemApi;
WatchApi watchApi;
RtcApi rtcApi;
Manager manager;

SupervisorParameters p;

long time() // todo name convention for common and local mock funcs
{
    return timeResult;
}

void supervisorSleepStub(void *p, unsigned int sleepTimeSec)
{
    goToSleepCalled = true;
    sleepTimeSecValue = sleepTimeSec;
}

void supervisorSleepSomeTimeStub(void *p, unsigned int sleepTimeSec)
{
    goToSleepCalled = true;
    sleepTimeSecValue = sleepTimeSec;
    timeResult = 48; // to simulate time pass
}

Date getDateStub()
{
    return dateResult;
}

unsigned int getNextWakeUpPeriodStub()
{
    return nextWakeUpPeriod;
}

void setUp(void)
{
    timeResult = 0;
    goToSleepCalled = false;
    sleepTimeSecValue = 0;
    lastUserEventTimestamp = 0;
    goToSleepTime = 10;
    nextWakeUpPeriod = NW_DONT_CARE;

    systemApi = systemApiMock();
    systemApi.time = time;

    watchApi = watchApiMock();
    rtcApi = rtcApiMock();
    manager ={
        .getNextWakeUpPeriod = getNextWakeUpPeriodStub 
    };

    p = {
        .watchMutex = &watchMutex,
        .lastUserEventTimestamp = &lastUserEventTimestamp,
        .goToSleepTime = goToSleepTime,
        .supervisorSleep = supervisorSleepStub,
        .systemApi = &systemApi,
        .watchApi = &watchApi,
        .rtcApi = &rtcApi,
        .manager = &manager,
    };
}

//platformio test -v --environment native --filter "*_super*"

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
    timeResult = lastUserEventTimestamp + p.goToSleepTime + 1; // after configured go to sleep time

    supervisor(&p);

    TEST_ASSERT_TRUE(goToSleepCalled); // THEN go to sleep
    unsigned int sleepTime = 3600 - 1 - 3; // (hour in sec) - (1sec from dateResult.second) - (supervisor threshold)
    TEST_ASSERT_EQUAL_UINT32(sleepTime, sleepTimeSecValue); // THEN for sleep calculated time
}

void whenActionModeAndIdleTimePassedButManagerReturnsNoSleep()
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
    
    nextWakeUpPeriod = NW_NO_SLEEP;
    lastUserEventTimestamp = 1;
    timeResult = lastUserEventTimestamp + p.goToSleepTime + 1; // after configured go to sleep time

    supervisor(&p);

    TEST_ASSERT_FALSE(goToSleepCalled); // THEN go to sleep
}

void whenActionModeAndIdleTimePassedButManagerReturnsTimeLesserToNextHour()
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
    
    nextWakeUpPeriod = 48 * 60;
    lastUserEventTimestamp = 1;
    timeResult = lastUserEventTimestamp + p.goToSleepTime + 1; // after configured go to sleep time

    supervisor(&p);

    TEST_ASSERT_FALSE(goToSleepCalled); // THEN go to sleep
    unsigned int sleepTime = 3600 - 1 - 3; // (hour in sec) - (1sec from dateResult.second) - (supervisor threshold)
    TEST_ASSERT_EQUAL_UINT32(sleepTime, sleepTimeSecValue); // THEN for sleep calculated time
}

void whenAfterWakeUp()
{
    lastUserEventTimestamp = 1;
    timeResult = 15;
    p.supervisorSleep = supervisorSleepSomeTimeStub;

    supervisor(&p);

    TEST_ASSERT_EQUAL_INT64(48, lastUserEventTimestamp); // THEN last event timestamp updated
}

void whenActionModeAndIdleTimeNotPassed()
{
    lastUserEventTimestamp = 1;
    timeResult = 5;

    supervisor(&p);

    TEST_ASSERT_FALSE(goToSleepCalled); // THEN stay in action
}

void whenSleepModeAndEvent()
{
    lastUserEventTimestamp = 1;
    timeResult = 2;

    supervisor(&p);

    TEST_ASSERT_FALSE(goToSleepCalled); // THEN wake up
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

    TEST_ASSERT_FALSE(goToSleepCalled); // THEN no sleep
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

    TEST_ASSERT_TRUE(goToSleepCalled); // THEN sleep
    TEST_ASSERT_EQUAL_UINT32(7, sleepTimeSecValue); // THEN for 7 second
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenActionModeAndIdleTimePassed);
    RUN_TEST(whenActionModeAndIdleTimePassedButManagerReturnsNoSleep);
    RUN_TEST(whenActionModeAndIdleTimePassedButManagerReturnsTimeLesserToNextHour);
    RUN_TEST(whenAfterWakeUp);
    RUN_TEST(whenActionModeAndIdleTimeNotPassed);
    RUN_TEST(whenSleepModeAndEvent);
    RUN_TEST(whenSleepTimeLesserThanGotoSleepPeriod);
    RUN_TEST(whenSleepTimeGreaterThanGotoSleepPeriod);
    UNITY_END();
}