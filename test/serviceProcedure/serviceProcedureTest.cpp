#include <unity.h>

#include "../systemMock.hpp"
#include "../bmaMock.hpp"
#include "../rtcMock.hpp"
#include "../motorMock.hpp"

#include "task/serviceProcedure.cpp"
#include "tools/tools.cpp"

RtcApi rtcApi;
BmaApi bmaApi;
SystemApi systemApi;
MotorApi motorApi;

int watchMutex;
bool resetStepCounterCalled;
Date dateResult;
int buzzDuration;

ServiceProcedureParameters p;

bool resetStepCounterStub()
{
    resetStepCounterCalled = true;
    return true;
}

Date getDateStub()
{
    return dateResult;
}

void buzzStub(int duration)
{
    buzzDuration = duration;
}

void setUp(void)
{
    resetStepCounterCalled = false;
    buzzDuration = 0;
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
    motorApi = motorApiMock();
    motorApi.buzz = buzzStub;

    p ={
        .watchMutex = &watchMutex,
        .lastBuzzHour = 0,
        .lastStepCounterResetDay = 0,
        .rtcApi = &rtcApi,
        .bmaApi = &bmaApi,
        .systemApi = &systemApi,
        .motorApi = &motorApi,
    };
}

void whenPeriodStartAndLastResetWasDayAgo()
{
    p.lastStepCounterResetDay = 18;
    serviceProcedure(&p);  

    TEST_ASSERT_EQUAL_UINT8(1, resetStepCounterCalled); // THEN step counter reset  
    TEST_ASSERT_EQUAL_UINT8(19, p.lastStepCounterResetDay); // THEN last reset value set to current day  
}

void whenPeriodMiddleAndLastResetWasDayAgo()
{
    dateResult.hour = 15;
    dateResult.minute = 48;
    p.lastStepCounterResetDay = 18;
    serviceProcedure(&p);  

    TEST_ASSERT_EQUAL_UINT8(0, resetStepCounterCalled); // THEN no step counter reset  
    TEST_ASSERT_EQUAL_UINT8(18, p.lastStepCounterResetDay); // THEN last reset value not changed  
}

void whenPeriodStartAndLastResetWasInCurrentDay()
{
    p.lastStepCounterResetDay = 19;
    serviceProcedure(&p);  

    TEST_ASSERT_EQUAL_UINT8(0, resetStepCounterCalled); // THEN no step counter reset  
    TEST_ASSERT_EQUAL_UINT8(19, p.lastStepCounterResetDay); // THEN last reset value not changed  
}

void whenHourStartAndLastBuzzHourWasBefore()
{
    p.lastBuzzHour = 20;
    serviceProcedure(&p);

    //TEST_ASSERT_EQUAL_UINT32(100, buzzDuration); // THEN buzz called
    TEST_ASSERT_EQUAL_UINT8(21, p.lastBuzzHour); // THEN last buzz value set to current hour  
}

void whenHourStartAndLastBuzzHourIsCurrent()
{
    p.lastBuzzHour = 21;
    serviceProcedure(&p);

    TEST_ASSERT_EQUAL_UINT32(0, buzzDuration); // THEN buzz not called
    TEST_ASSERT_EQUAL_UINT8(21, p.lastBuzzHour); // THEN last buzz value not changed  
}

void whenHourMiddleAndLastBuzzHourIsCurrent()
{
    dateResult.minute = 48;
    p.lastBuzzHour = 21;
    serviceProcedure(&p);

    TEST_ASSERT_EQUAL_UINT32(0, buzzDuration); // THEN buzz not called
    TEST_ASSERT_EQUAL_UINT8(21, p.lastBuzzHour); // THEN last buzz value not changed  
}

void whenHourMiddleAndLastBuzzHourIsNotCurrent()
{
    dateResult.minute = 48;
    p.lastBuzzHour = 20;
    serviceProcedure(&p);

    TEST_ASSERT_EQUAL_UINT32(0, buzzDuration); // THEN buzz not called
    TEST_ASSERT_EQUAL_UINT8(20, p.lastBuzzHour); // THEN last buzz value not changed  
}

void whenNoBuzzAllowedHour()
{
    p.lastBuzzHour = 4;
    dateResult.hour = 5;
    serviceProcedure(&p);

    TEST_ASSERT_EQUAL_UINT32(0, buzzDuration); // THEN buzz not called
    TEST_ASSERT_EQUAL_UINT8(4, p.lastBuzzHour); // THEN last buzz value not changed
}

void whenBuzzAllowedHour()
{
    p.lastBuzzHour = 21;
    dateResult.hour = 22;
    serviceProcedure(&p);

    //TEST_ASSERT_EQUAL_UINT32(100, buzzDuration); // THEN buzz called
    TEST_ASSERT_EQUAL_UINT8(22, p.lastBuzzHour); // THEN last buzz value set to current hour  
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenPeriodStartAndLastResetWasDayAgo);
    RUN_TEST(whenPeriodMiddleAndLastResetWasDayAgo);
    RUN_TEST(whenPeriodStartAndLastResetWasInCurrentDay);
    RUN_TEST(whenHourStartAndLastBuzzHourWasBefore);
    RUN_TEST(whenHourStartAndLastBuzzHourIsCurrent);
    RUN_TEST(whenHourMiddleAndLastBuzzHourIsCurrent);
    RUN_TEST(whenHourMiddleAndLastBuzzHourIsNotCurrent);
    RUN_TEST(whenNoBuzzAllowedHour);
    RUN_TEST(whenBuzzAllowedHour);
    UNITY_END();
}