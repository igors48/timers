#include <unity.h>

#include "../systemMock.hpp"
#include "../bmaMock.hpp"
#include "../rtcMock.hpp"

#include "task/stepCounterReset.cpp"
#include "tools/tools.cpp"

RtcApi rtcApi;
BmaApi bmaApi;
SystemApi systemApi;

int watchMutex;
bool resetStepCounterCalled;
Date dateResult;

StepCounterResetParameters p;

bool resetStepCounterStub()
{
    resetStepCounterCalled = true;
    return true;
}

Date getDateStub()
{
    return dateResult;
}

void setUp(void)
{
    resetStepCounterCalled = false;
    dateResult = {
        .year = 2021,
        .month = 8,
        .day = 19,
        .hour = 21,
        .minute = 0,
        .second = 0, // start of the the new hour 
    };

    rtcApi = rtcApiMock();
    rtcApi.getDate = getDateStub;
    bmaApi = bmaApiMock();
    bmaApi.resetStepCounter = resetStepCounterStub;
    systemApi = systemApiMock();

    p ={
        .watchMutex = &watchMutex,
        .lastReset = 0,
        .rtcApi = &rtcApi,
        .bmaApi = &bmaApi,
        .systemApi = &systemApi,
    };
}

void whenPeriodStartAndLastResetWasDayAgo()
{
    p.lastReset = 18;
    stepCounterReset(&p);  

    TEST_ASSERT_EQUAL_UINT8(1, resetStepCounterCalled); // THEN step counter reset  
    TEST_ASSERT_EQUAL_UINT8(19, p.lastReset); // THEN last reset value set to current day  
}

void whenPeriodMiddleAndLastResetWasDayAgo()
{
    dateResult.hour = 15;
    dateResult.minute = 48;
    p.lastReset = 18;
    stepCounterReset(&p);  

    TEST_ASSERT_EQUAL_UINT8(0, resetStepCounterCalled); // THEN no step counter reset  
    TEST_ASSERT_EQUAL_UINT8(18, p.lastReset); // THEN last reset value not changed  
}

void whenPeriodStartAndLastResetWasInCurrentDay()
{
    p.lastReset = 19;
    stepCounterReset(&p);  

    TEST_ASSERT_EQUAL_UINT8(0, resetStepCounterCalled); // THEN no step counter reset  
    TEST_ASSERT_EQUAL_UINT8(19, p.lastReset); // THEN last reset value not changed  
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenPeriodStartAndLastResetWasDayAgo);
    RUN_TEST(whenPeriodMiddleAndLastResetWasDayAgo);
    RUN_TEST(whenPeriodStartAndLastResetWasInCurrentDay);
    UNITY_END();
}