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
    unsigned int duration = 2;
    unsigned int tickCount = 3; 
    
    timeKeeperReset(&keeper, duration, tickCount);

    TEST_ASSERT_EQUAL_UINT32(duration, keeper.duration); // THEN ticker parameters set to corresponding values
    TEST_ASSERT_EQUAL_UINT32(tickCount, keeper.lastTick);
}

void whenTimeKeeperTickBeforeDurationEnd()
{
    unsigned int duration = 20;
    unsigned int tickCount = 300; 

    timeKeeperReset(&keeper, duration, tickCount);

    unsigned int durationRest = 15;
    unsigned int tickBeforeDurationEnd = tickCount + (duration - durationRest);

    timeKeeperTick(&keeper, tickBeforeDurationEnd);

    TEST_ASSERT_EQUAL_UINT32(durationRest, keeper.duration); // THEN duration set to rest
    TEST_ASSERT_EQUAL_UINT32(tickBeforeDurationEnd, keeper.lastTick); // THEN last tick set to current tick
}

void whenTimeKeeperTickAfterDurationEnd()
{
    unsigned int duration = 20;
    unsigned int tickCount = 300; 

    timeKeeperReset(&keeper, duration, tickCount);

    unsigned int sometimeAfter = 15;
    unsigned int tickAfterDurationEnd = tickCount + duration + sometimeAfter;

    timeKeeperTick(&keeper, tickAfterDurationEnd);

    TEST_ASSERT_EQUAL_UINT32(0, keeper.duration); // THEN duration set to rest
    TEST_ASSERT_EQUAL_UINT32(tickAfterDurationEnd, keeper.lastTick); // THEN last tick set to current tick
}

void whenTimeKeeperTickAtTheDurationEnd()
{
    unsigned int duration = 20;
    unsigned int tickCount = 300; 

    timeKeeperReset(&keeper, duration, tickCount);

    unsigned int tickBeforeDurationEnd = tickCount + duration;

    timeKeeperTick(&keeper, tickBeforeDurationEnd);

    TEST_ASSERT_EQUAL_UINT32(0, keeper.duration); // THEN duration set to rest
    TEST_ASSERT_EQUAL_UINT32(tickBeforeDurationEnd, keeper.lastTick); // THEN last tick set to current tick
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