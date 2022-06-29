#include <unity.h>

#include "core/app/ticker.cpp"

#include "../systemMock.hpp"

TickerParameters p;
SystemApi systemApi;
int watchMutex;
unsigned long tickSent;
unsigned int delaySet;

const unsigned int DELAY = 524;
const unsigned long TICK_COUNT = 7532;

void tickerFunc(unsigned long tick)
{
    tickSent = tick;
}

unsigned long getTickCountStub()
{
    return TICK_COUNT;
}

void delayStub(unsigned int time)
{
    delaySet = time;
}

void setUp(void)
{
    tickSent = 0;
    delaySet = 0;

    systemApi = systemApiMock();
    systemApi.getTickCount = getTickCountStub;
    systemApi.delay = delayStub;

    p = {
        .name = (char *)"name",
        .watchMutex = &watchMutex,
        .delayMs = DELAY,
        .func = tickerFunc,
        .systemApi = &systemApi,
    };
}

//platformio test --environment native --filter "test_ticker"

void whenTickerTicks()
{
    ticker((void *)(&p));

    TEST_ASSERT_EQUAL_UINT64(TICK_COUNT, tickSent); // THEN tick count is sent to the consumer
    TEST_ASSERT_EQUAL_UINT32(DELAY, delaySet);      // THEN delay set according to parameters
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTickerTicks);
    UNITY_END();
}