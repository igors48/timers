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
unsigned short goToSleepTime;
unsigned int nextWakeUpPeriod;
WakeUpReason wakeUpReason;

int watchMutex;

SystemApi systemApi;
WatchApi watchApi;
RtcApi rtcApi;
Manager manager;

SupervisorParameters p;
SupervisorApi supervisorApi;

long time() // todo name convention for common and local mock funcs
{
    return timeResult;
}

WakeUpReason supervisorSleepStub(void *p, unsigned long sleepTimeMillis)
{
    goToSleepCalled = true;
    sleepTimeSecValue = sleepTimeMillis;
    return wakeUpReason;
}

WakeUpReason supervisorSleepSomeTimeStub(void *p, unsigned long sleepTimeMillis)
{
    goToSleepCalled = true;
    sleepTimeSecValue = sleepTimeMillis;
    timeResult = 48; // to simulate time pass
    return wakeUpReason;
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
    wakeUpReason = WUR_SLEEP_WAKEUP_TIMER;

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

    supervisorApi = watchSupervisorApi();
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
    unsigned int sleepTime = (3600 - 1 - 3) * 1000; // ((hour in sec) - (1sec from dateResult.second) - (supervisor threshold)) => millis
    TEST_ASSERT_EQUAL_UINT32(sleepTime, sleepTimeSecValue); // THEN for sleep calculated time
    TEST_ASSERT_EQUAL_INT64(timeResult, lastUserEventTimestamp); // THEN last user event timestamp updated
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

    TEST_ASSERT_FALSE(goToSleepCalled); // THEN no sleep
    TEST_ASSERT_EQUAL_INT64(timeResult, lastUserEventTimestamp); // THEN last user event timestamp updated
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
    
    nextWakeUpPeriod = 48 * 60 * 1000;
    lastUserEventTimestamp = 1;
    timeResult = lastUserEventTimestamp + p.goToSleepTime + 1; // after configured go to sleep time

    supervisor(&p);

    TEST_ASSERT_TRUE(goToSleepCalled); // THEN go to sleep
    unsigned int sleepTime = nextWakeUpPeriod - 3 * 1000; // (next wake up period) - (supervisor threshold)
    TEST_ASSERT_EQUAL_UINT32(sleepTime, sleepTimeSecValue); // THEN for sleep calculated time
    TEST_ASSERT_EQUAL_UINT32(nextWakeUpPeriod, supervisorApi.getNextWakeUpPeriod()); // THEN next wake up period returned by api
    TEST_ASSERT_EQUAL_INT64(timeResult, lastUserEventTimestamp); // THEN last user event timestamp updated
}

void whenAfterWakeUp()
{
    lastUserEventTimestamp = 14;
    timeResult = 15;
    p.supervisorSleep = supervisorSleepSomeTimeStub;

    supervisor(&p);

    TEST_ASSERT_EQUAL_INT64(14, lastUserEventTimestamp); // THEN last event timestamp updated
    TEST_ASSERT_TRUE(wakeUpReason == supervisorApi.getWakeUpReason()); // THEN last wake up reason returned by the api
    TEST_ASSERT_EQUAL_UINT16(9, supervisorApi.getTimeToSleep()); // THEN time to sleep (goToSleepTime - (timeResult - lastUserEventTimestamp)) returned by api
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
    TEST_ASSERT_EQUAL_INT64(timeResult, lastUserEventTimestamp); // THEN last user event timestamp updated
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
    TEST_ASSERT_EQUAL_UINT32(7000, sleepTimeSecValue); // THEN for 7000 milliseconds
    TEST_ASSERT_EQUAL_INT64(timeResult, lastUserEventTimestamp); // THEN last user event timestamp updated
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