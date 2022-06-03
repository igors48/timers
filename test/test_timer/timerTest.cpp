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

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.timeKeeper.duration); // THEN keeper duration set to alarm duration
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.timeKeeper.lastTick); // THEN last tick updated to current tick
    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN state set to ALARM

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration); // THEN other values - no change   

    TEST_ASSERT_TRUE(beepCalled); // THEN first beep
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.lastBeep); // THEN last beep tick set to current
}

void whenTickInRunModeButDurationPassedPrecisely()
{
    timerStart(&timer, DURATION, INITIAL_TICKS);

    unsigned int currentTick = INITIAL_TICKS + DURATION;
    timerTick(&timer, currentTick);

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.timeKeeper.duration); // THEN keeper duration set to alarm duration
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.timeKeeper.lastTick); // THEN last tick updated to current tick
    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN state set to ALARM

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration); // THEN other values - no change   

    TEST_ASSERT_TRUE(beepCalled); // THEN first beep
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.lastBeep); // THEN last beep tick set to current
}

void gotoAlarmMode()
{
    timerStart(&timer, DURATION, INITIAL_TICKS);

    unsigned int currentTick = INITIAL_TICKS + DURATION;
    timerTick(&timer, currentTick);

    beepCalled = false; // reset after first beep (when alarm mode starts)
}

void whenTickInAlarmModeButNoTimeToBeep()
{
    gotoAlarmMode();

    unsigned int delta = BEEP_PAUSE - 125;
    unsigned int currentTick = INITIAL_TICKS + DURATION + delta;
    timerTick(&timer, currentTick);

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION - delta, timer.timeKeeper.duration); // THEN keeper duration set to alarm duration
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.timeKeeper.lastTick); // THEN last tick updated to current tick
    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN state set to ALARM

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration); // THEN other values - no change   

    TEST_ASSERT_FALSE(beepCalled); // THEN no beep
    TEST_ASSERT_EQUAL_UINT32(INITIAL_TICKS + DURATION, timer.lastBeep); // THEN last beep tick not changed
}

void whenTickInAlarmModeAndTimeToBeep()
{
    gotoAlarmMode();

    unsigned int delta = BEEP_PAUSE + 117;
    unsigned int currentTick = INITIAL_TICKS + DURATION + delta;
    timerTick(&timer, currentTick);

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION - delta, timer.timeKeeper.duration); // THEN keeper duration set to alarm duration
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.timeKeeper.lastTick); // THEN last tick updated to current tick
    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN state set to ALARM

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration); // THEN other values - no change   

    TEST_ASSERT_TRUE(beepCalled); // THEN beep
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.lastBeep); // THEN last beep tick set to current tick
}

void whenTickInAlarmModeAndTimeToBeepPrecisely()
{
    gotoAlarmMode();

    unsigned int delta = BEEP_PAUSE;
    unsigned int currentTick = INITIAL_TICKS + DURATION + delta;
    timerTick(&timer, currentTick);

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION - delta, timer.timeKeeper.duration); // THEN keeper duration set to alarm duration
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.timeKeeper.lastTick); // THEN last tick updated to current tick
    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN state set to ALARM

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration); // THEN other values - no change   

    TEST_ASSERT_TRUE(beepCalled); // THEN beep
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.lastBeep); // THEN last beep tick set to current tick
}

void whenTickInAlarmModeAfterAlarmDuration()
{
    gotoAlarmMode();

    unsigned int delta = ALARM_DURATION + 37;
    unsigned int currentTick = INITIAL_TICKS + DURATION + delta;
    timerTick(&timer, currentTick);

    TEST_ASSERT_EQUAL_UINT32(0, timer.timeKeeper.duration); // THEN keeper duration set to alarm duration
    TEST_ASSERT_EQUAL_UINT32(currentTick, timer.timeKeeper.lastTick); // THEN last tick updated to current tick
    TEST_ASSERT_TRUE(timer.state == TMS_IDLE); // THEN state set to ALARM

    TEST_ASSERT_EQUAL_UINT32(ALARM_DURATION, timer.alarmDuration); // THEN other values - no change   

    TEST_ASSERT_FALSE(beepCalled); // THEN no beep
    TEST_ASSERT_EQUAL_UINT32(INITIAL_TICKS + DURATION, timer.lastBeep); // THEN last beep tick no changed
}

void whenTimerStopInRunMode()
{
    timerStart(&timer, DURATION, INITIAL_TICKS);

    TimerResponse response = timerStop(&timer);

    TEST_ASSERT_TRUE(timer.state == TMS_IDLE); // THEN state set to IDLE
}

void whenTimerStopInAlarmMode()
{
    gotoAlarmMode();

    TimerResponse response = timerStop(&timer);

    TEST_ASSERT_TRUE(timer.state == TMS_IDLE); // THEN state set to IDLE
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(whenTimerCreated);
    RUN_TEST(whenTimerStarted);
    RUN_TEST(whenTimerStartedInRunMode);

    RUN_TEST(whenTickInRunMode);
    RUN_TEST(whenTickInRunModeButDurationPassed);
    RUN_TEST(whenTickInRunModeButDurationPassedPrecisely);

    RUN_TEST(whenTickInAlarmModeButNoTimeToBeep);
    RUN_TEST(whenTickInAlarmModeAndTimeToBeep);
    RUN_TEST(whenTickInAlarmModeAndTimeToBeepPrecisely);
    RUN_TEST(whenTickInAlarmModeAfterAlarmDuration);

    RUN_TEST(whenTimerStopInRunMode);
    RUN_TEST(whenTimerStopInAlarmMode);

    UNITY_END();
}