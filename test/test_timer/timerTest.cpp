#include <unity.h>

#include "apps/timer/timer.cpp"

const unsigned short PERIOD = 48;
const unsigned short COUNTER = 24;
const unsigned char DURATION = 10;

Timer timer;

void setUp(void)
{
    timer = {
        .period = PERIOD,
        .counter = COUNTER,
        .alarmDuration = DURATION,
        .state = TMS_IDLE,
    };
}

//platformio test -v --environment native --filter "*_timer*"

void whenSetPeriodInRunMode()
{
    timer.state = TMS_RUN;

    TEST_ASSERT_TRUE(setPeriod(&timer, 96) == TMR_ERROR); // THEN error returns

    TEST_ASSERT_TRUE(timer.state == TMS_RUN); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenSetPeriodInAlarmMode()
{
    timer.state = TMS_ALARM;

    TEST_ASSERT_TRUE(setPeriod(&timer, 96) == TMR_ERROR); // THEN error returns

    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenSetPeriodInIdleMode()
{
    unsigned short newPeriod = 96;
    
    TEST_ASSERT_TRUE(setPeriod(&timer, newPeriod) == TMR_OK); // THEN ok returns
    TEST_ASSERT_EQUAL_UINT16(newPeriod, timer.period); // THEN period updated
    TEST_ASSERT_TRUE(timer.state == TMS_IDLE); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}
/*
void whenStartInIdleMode()
{
    TEST_ASSERT_TRUE(start(&timer) == TMR_OK); // THEN ok returns

    TEST_ASSERT_TRUE(timer.state == TMS_RUN); // THEN state set to run
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenStartInRunMode()
{
    timer.state = TMS_RUN;

    TEST_ASSERT_TRUE(start(&timer) == TMR_OK); // THEN ok returns

    TEST_ASSERT_TRUE(timer.state == TMS_RUN); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenStartInAlarmMode()
{
    timer.state = TMS_ALARM;

    TEST_ASSERT_TRUE(start(&timer) == TMR_ERROR); // THEN error returns

    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenStopInIdleMode()
{
    TEST_ASSERT_TRUE(stop(&timer) == TMR_OK); // THEN ok returns

    TEST_ASSERT_TRUE(timer.state == TMS_IDLE); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenStopInRunMode()
{
    timer.state = TMS_RUN;

    TEST_ASSERT_TRUE(stop(&timer) == TMR_OK); // THEN ok returns

    TEST_ASSERT_TRUE(timer.state == TMS_IDLE); // THEN state set to idle
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenStopInAlarmMode()
{
    timer.state = TMS_ALARM;

    TEST_ASSERT_TRUE(stop(&timer) == TMR_ERROR); // THEN error returns

    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenResetInIdleMode()
{
    TEST_ASSERT_TRUE(reset(&timer) == TMR_OK); // THEN ok returns

    TEST_ASSERT_EQUAL_UINT16(timer.period, timer.counter); // THEN counter set to period 
    TEST_ASSERT_TRUE(timer.state == TMS_IDLE); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenResetInRunMode()
{
    timer.state = TMS_RUN;

    TEST_ASSERT_TRUE(reset(&timer) == TMR_ERROR); // THEN error returns

    TEST_ASSERT_TRUE(timer.state == TMS_RUN); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}

void whenResetInAlarmMode()
{
    timer.state = TMS_ALARM;

    TEST_ASSERT_TRUE(reset(&timer) == TMR_ERROR); // THEN error returns

    TEST_ASSERT_TRUE(timer.state == TMS_ALARM); // THEN other values not changed
    TEST_ASSERT_EQUAL_UINT16(PERIOD, timer.period); 
    TEST_ASSERT_EQUAL_UINT16(COUNTER, timer.counter);
    TEST_ASSERT_EQUAL_UINT8(DURATION, timer.alarmDuration);
}
*/
int main()
{
    UNITY_BEGIN();

    RUN_TEST(whenSetPeriodInIdleMode);
    RUN_TEST(whenSetPeriodInRunMode);
    RUN_TEST(whenSetPeriodInAlarmMode);
    
    // RUN_TEST(whenStartInIdleMode);
    // RUN_TEST(whenStartInRunMode);
    // RUN_TEST(whenStartInAlarmMode);
    
    // RUN_TEST(whenStopInIdleMode);
    // RUN_TEST(whenStopInRunMode);
    // RUN_TEST(whenStopInAlarmMode);
    
    // RUN_TEST(whenResetInIdleMode);
    // RUN_TEST(whenResetInRunMode);
    // RUN_TEST(whenResetInAlarmMode);

    UNITY_END();
}