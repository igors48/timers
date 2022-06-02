#include <unity.h>

#include "apps/timer/timer.cpp"
#include "apps/timer/timeKeeper.cpp"

const unsigned int DURATION = 2345;
const unsigned int INITIAL_TICKS = 48;

Timer timer;

bool beepCalled;

void beepStub()
{
    beepCalled = true;
}

void setUp(void)
{
    beepCalled = false;
    SoundApi soundApi = {
        .beep = beepStub,
    };

    timer = timerCreate(&soundApi);
}

//platformio test -v --environment native --filter "*_timer*"

void whenTimerCreated()
{
    TEST_ASSERT_EQUAL_UINT32(0, timer.timeKeeper.duration); // THEN all values set to default
    TEST_ASSERT_EQUAL_UINT32(0, timer.timeKeeper.lastTick);
    TEST_ASSERT_TRUE(timer.state == TMS_IDLE);

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration);

    TEST_ASSERT_EQUAL_UINT32(0, timer.lastBeep);
    TEST_ASSERT_FALSE(beepCalled); // THEN no beeps
}

void whenTimerStarted()
{
    TimerResponse response = timerStart(&timer, DURATION, INITIAL_TICKS);

    TEST_ASSERT_TRUE(TMR_OK == response); // THEN response is OK
    
    TEST_ASSERT_EQUAL_UINT32(DURATION, timer.timeKeeper.duration); // THEN time keeper initialized
    TEST_ASSERT_EQUAL_UINT32(INITIAL_TICKS, timer.timeKeeper.lastTick);
    TEST_ASSERT_TRUE(timer.state == TMS_RUN); // THEN timer state changed to RUN
    
    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration); // THEN other values - no change
    
    TEST_ASSERT_EQUAL_UINT32(0, timer.lastBeep);
    TEST_ASSERT_FALSE(beepCalled); // THEN no beeps
}

void whenTimerStartedInRunMode()
{
    timerStart(&timer, DURATION, INITIAL_TICKS);

    TimerResponse response = timerStart(&timer, DURATION + 34, INITIAL_TICKS + 8);
    
    TEST_ASSERT_TRUE(TMR_ERROR == response); // THEN response is ERROR
    
    TEST_ASSERT_EQUAL_UINT32(DURATION, timer.timeKeeper.duration); // THEN other values - no change
    TEST_ASSERT_EQUAL_UINT32(INITIAL_TICKS, timer.timeKeeper.lastTick);
    TEST_ASSERT_TRUE(timer.state == TMS_RUN);
    
    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration);
    
    TEST_ASSERT_EQUAL_UINT32(0, timer.lastBeep);
    TEST_ASSERT_FALSE(beepCalled); // THEN no beeps
}

void whenTickInRunMode()
{
    timerStart(&timer, DURATION, INITIAL_TICKS);

    unsigned int delta = 216;
    unsigned int currentTick = INITIAL_TICKS + delta;
    timerTick(&timer, currentTick);

    TEST_ASSERT_EQUAL_UINT32(DURATION - delta, timer.timeKeeper.duration); // THEN duration decreased to delta
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.timeKeeper.lastTick); // THEN last tick updated to current tick
    TEST_ASSERT_TRUE(timer.state == TMS_RUN); // THEN other values - no change
    
    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration);
    
    TEST_ASSERT_EQUAL_UINT32(0, timer.lastBeep);
    TEST_ASSERT_FALSE(beepCalled); // THEN no beeps
}

void whenTickInRunModeButDurationPassed()
{
    timerStart(&timer, DURATION, INITIAL_TICKS);

    unsigned int currentTick = INITIAL_TICKS + DURATION + 3;
    timerTick(&timer, currentTick);

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.timeKeeper.duration); // THEN duration decreased to delta
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.timeKeeper.lastTick); // THEN last tick updated to current tick
    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN state set to ALARM

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration); // THEN other values - no change   

    TEST_ASSERT_TRUE(beepCalled); // THEN first beep
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.lastBeep); // THEN last beep tick set to current
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(whenTimerCreated);
    RUN_TEST(whenTimerStarted);
    RUN_TEST(whenTimerStartedInRunMode);
    RUN_TEST(whenTickInRunMode);
    RUN_TEST(whenTickInRunModeButDurationPassed);

    UNITY_END();
}