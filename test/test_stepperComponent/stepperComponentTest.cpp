#include <unity.h>

#include "../systemMock.hpp"

#include "core/component/factory.cpp"
#include "core/component/group.cpp"
#include "core/component/component.cpp"
#include "core/component/buttonComponent.cpp"
#include "core/component/textComponent.cpp"
#include "core/component/stepperComponent.cpp"

const signed short MIN = 1;
const signed short MAX = 10;
const signed short INITIAL_VALUE = 5;
const signed short NOT_UPDATED = -1;

Factory factory;
StepperComponentState *state;
ButtonComponentState *plusButton;
ButtonComponentState *minusButton;
signed short updatedValue;

void onStepperChange(signed short value)
{
    updatedValue = value;
}

void setUp(void)
{
    SystemApi systemApi = systemApiMock();
    factory = createFactory(&systemApi);

    updatedValue = NOT_UPDATED;

    state = factory.createStepperComponentStateRef(MIN, MAX, INITIAL_VALUE, onStepperChange);
    plusButton = (ButtonComponentState *)(state->plusButton->state);
    minusButton = (ButtonComponentState *)(state->minusButton->state);
}

void whenSetValue()
{
    state->setValue(state, INITIAL_VALUE + 2);

    TEST_ASSERT_EQUAL_INT16(INITIAL_VALUE + 2, state->value); // THEN value changed
    TEST_ASSERT_TRUE(plusButton->mode == BM_ENABLED); // THEN plus button enabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_ENABLED); // THEN minus button enabled
    TEST_ASSERT_EQUAL_INT16(INITIAL_VALUE + 2, updatedValue); // THEN handler called
}

void whenSetValueToMax()
{
    state->setValue(state, MAX);

    TEST_ASSERT_EQUAL_INT16(MAX, state->value); // THEN value changed
    TEST_ASSERT_TRUE(plusButton->mode == BM_DISABLED); // THEN plus button disabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_ENABLED); // THEN minus button enabled
    TEST_ASSERT_EQUAL_INT16(MAX, updatedValue); // THEN handler called
}

void whenSetValueToMin()
{
    state->setValue(state, MIN);

    TEST_ASSERT_EQUAL_INT16(MIN, state->value); // THEN value changed
    TEST_ASSERT_TRUE(plusButton->mode == BM_ENABLED); // THEN plus button enabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_DISABLED); // THEN minus button disabled
    TEST_ASSERT_EQUAL_INT16(MIN, updatedValue); // THEN handler called
}

void whenMinus()
{
    stepperOnMinus(state);

    TEST_ASSERT_EQUAL_INT16(INITIAL_VALUE - 1, state->value); // THEN value decreased
    TEST_ASSERT_TRUE(plusButton->mode == BM_ENABLED); // THEN plus button enabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_ENABLED); // THEN minus button enabled
    TEST_ASSERT_EQUAL_INT16(INITIAL_VALUE - 1, updatedValue); // THEN handler called
}

void whenMinusToMin()
{
    state->setValue(state, MIN + 1);
    stepperOnMinus(state);

    TEST_ASSERT_EQUAL_INT16(MIN, state->value); // THEN value decreased
    TEST_ASSERT_TRUE(plusButton->mode == BM_ENABLED); // THEN plus button enabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_DISABLED); // THEN minus button disabled
    TEST_ASSERT_EQUAL_INT16(MIN, updatedValue); // THEN handler called
}

void whenMinusIfMin()
{
    state->setValue(state, MIN);
    updatedValue = NOT_UPDATED; // reset listener
    stepperOnMinus(state);

    TEST_ASSERT_EQUAL_INT16(MIN, state->value); // THEN value not changed
    TEST_ASSERT_TRUE(plusButton->mode == BM_ENABLED); // THEN plus button enabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_DISABLED); // THEN minus button disabled
    TEST_ASSERT_EQUAL_INT16(NOT_UPDATED, updatedValue); // THEN handler not called
}

void whenPlus()
{
    stepperOnPlus(state);

    TEST_ASSERT_EQUAL_INT16(INITIAL_VALUE + 1, state->value); // THEN value increased
    TEST_ASSERT_TRUE(plusButton->mode == BM_ENABLED); // THEN plus button enabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_ENABLED); // THEN minus button enabled
    TEST_ASSERT_EQUAL_INT16(INITIAL_VALUE + 1, updatedValue); // THEN handler called
}

void whenPlusToMax()
{
    state->setValue(state, MAX - 1);
    stepperOnPlus(state);

    TEST_ASSERT_EQUAL_INT16(MAX, state->value); // THEN value increased
    TEST_ASSERT_TRUE(plusButton->mode == BM_DISABLED); // THEN plus button disabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_ENABLED); // THEN minus button enabled
    TEST_ASSERT_EQUAL_INT16(MAX, updatedValue); // THEN handler called
}

void whenPlusIfMax()
{
    state->setValue(state, MAX);
    updatedValue = NOT_UPDATED;
    stepperOnPlus(state);

    TEST_ASSERT_EQUAL_INT16(MAX, state->value); // THEN value not changed
    TEST_ASSERT_TRUE(plusButton->mode == BM_DISABLED); // THEN plus button disabled
    TEST_ASSERT_TRUE(minusButton->mode == BM_ENABLED); // THEN minus button enabled
    TEST_ASSERT_EQUAL_INT16(NOT_UPDATED, updatedValue); // THEN handler called
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenSetValue);
    RUN_TEST(whenSetValueToMax);
    RUN_TEST(whenSetValueToMin);
    RUN_TEST(whenMinus);
    RUN_TEST(whenMinusToMin);
    RUN_TEST(whenMinusIfMin);
    RUN_TEST(whenPlus);
    RUN_TEST(whenPlusToMax);
    RUN_TEST(whenPlusIfMax);
    UNITY_END();
}