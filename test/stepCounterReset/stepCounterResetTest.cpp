#include <unity.h>

#include "../systemMock.hpp"
#include "../bmaMock.hpp"
#include "../rtcMock.hpp"

#include "task/stepCounterReset.cpp"
#include "tools/tools.cpp"

RtcApi rtcApi;
BmaApi bmaApi;
SystemApi systemApi;

unsigned int lastWakeTime;
int watchMutex;
bool resetStepCounterCalled;
Date dateResult;
unsigned int prevoiusWakeTimeMillisValue; 
unsigned int timeIncrementMillisValue;

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

void delayUntilStub(unsigned int *prevoiusWakeTimeMillis, unsigned int timeIncrementMillis)
{
    prevoiusWakeTimeMillisValue = *prevoiusWakeTimeMillis;
    timeIncrementMillisValue = timeIncrementMillis;
}

void setUp(void)
{
    lastWakeTime = 42;
    resetStepCounterCalled = false;
    prevoiusWakeTimeMillisValue = 0; 
    timeIncrementMillisValue = 0;
    dateResult = {
        .year = 2021,
        .month = 8,
        .day = 19,
        .hour = 7,
        .minute = 0,
        .second = 2, // start of the the new hour 
    };

    rtcApi = rtcApiMock();
    rtcApi.getDate = getDateStub;
    bmaApi = bmaApiMock();
    bmaApi.resetStepCounter = resetStepCounterStub;
    systemApi = systemApiMock();
    systemApi.delayUntil = delayUntilStub;

    p ={
        .watchMutex = &watchMutex,
        .lastWakeTime = &lastWakeTime,
        .rtcApi = &rtcApi,
        .bmaApi = &bmaApi,
        .systemApi = &systemApi,
    };
}

void whenHappyFlow()
{
    stepCounterReset(&p);  

    TEST_ASSERT_EQUAL_UINT8(1, resetStepCounterCalled); // THEN step counter reset  
    TEST_ASSERT_EQUAL_UINT32(42, prevoiusWakeTimeMillisValue); // THEN previous wake up correctly set
    TEST_ASSERT_EQUAL_UINT32(3598000, timeIncrementMillisValue); // THEN next wake up scheduled to next hour -> (60 - 2) * 60 * 1000 millis
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenHappyFlow);
    UNITY_END();
}