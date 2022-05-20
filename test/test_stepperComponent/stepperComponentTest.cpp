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

Factory factory;
StepperComponentState *state;

void onStepperChange(signed short value)
{

}

void setUp(void)
{
    SystemApi systemApi = systemApiMock();
    factory = createFactory(&systemApi);

    state = factory.createStepperComponentStateRef(MIN, MAX, INITIAL_VALUE, onStepperChange);
}

void whenSetValue()
{
    state->setValue(state, INITIAL_VALUE + 2);

    TEST_ASSERT_EQUAL_INT16(INITIAL_VALUE + 2, state->value); // THEN value changed
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenSetValue);
    UNITY_END();
}