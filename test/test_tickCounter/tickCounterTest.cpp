#include <unity.h>

#include "core/system/tickCounter.cpp"

const unsigned int INITIAL_VALUE = 385;

void setUp(void)
{
    resetTickCount(INITIAL_VALUE);
}

//platformio test --environment native --filter "*_tickCounter*"

void whenReset()
{
    TEST_ASSERT_EQUAL_UINT64(INITIAL_VALUE, tickCount); // THEN tick count set to initial value
    TEST_ASSERT_EQUAL_UINT32(INITIAL_VALUE, lastSystemTickCount); // THEN last system tick count set to initial value
}

void whenNewSystemTickValueGreaterThenLast() 
{
    unsigned int systemTickValue = INITIAL_VALUE + 1275;

    unsigned long tick = newTickCountValue(systemTickValue);
    TEST_ASSERT_EQUAL_UINT64(systemTickValue, tick);
}

void whenNewSystemTickValueLesserThenLast() 
{
    unsigned int systemTickValue = INITIAL_VALUE - 310; // 75

    unsigned long beforeOverflow = UINT_MAX - INITIAL_VALUE;
    unsigned long expectedTickValue = INITIAL_VALUE + beforeOverflow + systemTickValue; //4294967370

    unsigned long tick = newTickCountValue(systemTickValue);

    TEST_ASSERT_EQUAL_UINT8(1, overflowCount);
    TEST_ASSERT_EQUAL_UINT64(expectedTickValue, tick);
}

void whenSeveralOverflows() 
{
    unsigned int firstSystemTickValue = 75;
    unsigned int secondSystemTickValue = 47;

    newTickCountValue(firstSystemTickValue);
    unsigned long tick = newTickCountValue(secondSystemTickValue);

    unsigned long expectedTickValue = (unsigned long)2 * (unsigned long)UINT_MAX + secondSystemTickValue;

    TEST_ASSERT_EQUAL_UINT64(expectedTickValue, tick);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenReset);
    RUN_TEST(whenNewSystemTickValueGreaterThenLast);
    RUN_TEST(whenNewSystemTickValueLesserThenLast);
    RUN_TEST(whenSeveralOverflows);
    UNITY_END();
}
