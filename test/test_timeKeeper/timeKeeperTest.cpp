#include <unity.h>

#include "apps/timer/timeKeeper.cpp"

TimeKeeper keeper;

void setUp(void)
{
    keeper = timeKeeperCreate();
}

//platformio test -v --environment native --filter "test_timeKeeper*"

void whenTimeKeeperReset()
{
    const unsigned long duration = 2;
    const unsigned long tickCount = 3; 
    
    timeKeeperReset(&keeper, duration, tickCount);

    TEST_ASSERT_EQUAL_UINT64(duration, keeper.duration); // THEN ticker parameters set to corresponding values
    TEST_ASSERT_EQUAL_UINT64(tickCount, keeper.lastTick);
}

void whenTimeKeeperTickBeforeDurationEnd()
{
    const unsigned long duration = 20;
    const unsigned long tickCount = 300; 

    timeKeeperReset(&keeper, duration, tickCount);

    const unsigned long durationRest = 15;
    const unsigned long tickBeforeDurationEnd = tickCount + (duration - durationRest);

    timeKeeperTick(&keeper, tickBeforeDurationEnd);

    TEST_ASSERT_EQUAL_UINT64(durationRest, keeper.duration); // THEN duration set to rest
    TEST_ASSERT_EQUAL_UINT64(tickBeforeDurationEnd, keeper.lastTick); // THEN last tick set to current tick
}

void whenTimeKeeperTickAfterDurationEnd()
{
    const unsigned long duration = 20;
    const unsigned long tickCount = 300; 

    timeKeeperReset(&keeper, duration, tickCount);

    const unsigned long sometimeAfter = 15;
    const unsigned long tickAfterDurationEnd = tickCount + duration + sometimeAfter;

    timeKeeperTick(&keeper, tickAfterDurationEnd);

    TEST_ASSERT_EQUAL_UINT64(0, keeper.duration); // THEN duration set to rest
    TEST_ASSERT_EQUAL_UINT64(tickAfterDurationEnd, keeper.lastTick); // THEN last tick set to current tick
}

void whenTimeKeeperTickAtTheDurationEnd()
{
    const unsigned long duration = 20;
    const unsigned long tickCount = 300; 

    timeKeeperReset(&keeper, duration, tickCount);

    const unsigned long tickBeforeDurationEnd = tickCount + duration;

    timeKeeperTick(&keeper, tickBeforeDurationEnd);

    TEST_ASSERT_EQUAL_UINT64(0, keeper.duration); // THEN duration set to rest
    TEST_ASSERT_EQUAL_UINT64(tickBeforeDurationEnd, keeper.lastTick); // THEN last tick set to current tick
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTimeKeeperReset);
    RUN_TEST(whenTimeKeeperTickBeforeDurationEnd);
    RUN_TEST(whenTimeKeeperTickAtTheDurationEnd);
    RUN_TEST(whenTimeKeeperTickAfterDurationEnd);
    UNITY_END();
}