#include <unity.h>

#include "../rtcMock.hpp"

#include "core/tools/tools.cpp"

void secondsToNextHourStartTests()
{
    Date now = {
        .year = 2021,
        .month = 8,
        .day = 13,
        .hour = 7,
        .minute = 00,
        .second = 00,
    };
    unsigned short sleepTime = secondsToNextHourStart(now);
    TEST_ASSERT_EQUAL_UINT16(0, sleepTime);

    now.minute = 0;
    now.second = 1;
    sleepTime = secondsToNextHourStart(now);
    TEST_ASSERT_EQUAL_UINT16(59 * 60 + 59, sleepTime);

    now.minute = 0;
    now.second = 59;
    sleepTime = secondsToNextHourStart(now);
    TEST_ASSERT_EQUAL_UINT16(59 * 60 + 1, sleepTime);

    now.minute = 1;
    now.second = 0;
    sleepTime = secondsToNextHourStart(now);
    TEST_ASSERT_EQUAL_UINT16(58 * 60 + 60, sleepTime);

    now.minute = 59;
    now.second = 0;
    sleepTime = secondsToNextHourStart(now);
    TEST_ASSERT_EQUAL_UINT16(0 * 60 + 60, sleepTime);

    now.minute = 59;
    now.second = 59;
    sleepTime = secondsToNextHourStart(now);
    TEST_ASSERT_EQUAL_UINT16(0 * 60 + 1, sleepTime);

    now.minute = 59;
    now.second = 58;
    sleepTime = secondsToNextHourStart(now);
    TEST_ASSERT_EQUAL_UINT16(0 * 60 + 2, sleepTime);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(secondsToNextHourStartTests);
    UNITY_END();
}