#include <unity.h>

#include "../rtcMock.hpp"
#include "../systemMock.hpp"
#include "../powerMock.hpp"
#include "../bmaMock.hpp"

#include "task/watchStateProducer.cpp"

int watchMutex;
Date date;
WatchState watchState;
RtcApi rtcApi;
SystemApi systemApi;
PowerApi powerApi;
BmaApi bmaApi;

WatchStateProducerParameters p;

void setUp(void)
{
    date = {
        .year = 0,
        .month = 0,
        .day = 0,
        .hour = 0,
        .minute = 0,
        .second = 0};
    watchState = {
        .date = date};

    rtcApi = rtcApiMock();
    systemApi = systemApiMock();
    powerApi = powerApiMock();
    bmaApi = bmaApiMock();

    p = {
        .watchMutex = &watchMutex,
        .state = &watchState,
        .rtcApi = &rtcApi,
        .systemApi = &systemApi,
        .powerApi = &powerApi,
        .bmaApi = &bmaApi,
    };
}

void whenHappyFlow(void)
{
    watchStateProducer(&p);

    TEST_ASSERT_EQUAL_UINT16(1, watchState.date.year); // THEN all members set
    TEST_ASSERT_EQUAL_UINT8(2, watchState.date.month);
    TEST_ASSERT_EQUAL_UINT8(3, watchState.date.day);
    TEST_ASSERT_EQUAL_UINT8(4, watchState.date.hour);
    TEST_ASSERT_EQUAL_UINT8(5, watchState.date.minute);
    TEST_ASSERT_EQUAL_UINT8(6, watchState.date.second);
    TEST_ASSERT_EQUAL_INT(42, watchState.battPercentage);
    TEST_ASSERT_EQUAL_UINT32(100500, watchState.stepCount);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenHappyFlow);
    UNITY_END();
}