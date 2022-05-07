#include <unity.h>

#include "core/component/component.cpp"
#include "core/component/buttonComponent.cpp"

static char TEST[] = "test";
static bool handlerCalled;
static ButtonComponentState state;
static Component button;

void handlerStub()
{
    handlerCalled = true;
}

void setUp(void)
{
    handlerCalled = false;

    state = createButtonState(TEST, EG_ONCE, handlerStub);
    button = createButtonComponent(10, 10, 10, 10, &state);
}

void whenTouch()
{
    button.onTouch(&button, 15, 15, 48);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_PRESS); // THEN state is set to press
    TEST_ASSERT_EQUAL_UINT32(state.firstTouchTick, 48); // THEN first touch tick is stored
    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
}

void whenTouchDisabled()
{
    state.mode = BM_DISABLED;
    button.onTouch(&button, 15, 15, 48);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is not changed
    TEST_ASSERT_EQUAL_UINT32(state.firstTouchTick, 0); // THEN first touch tick is not changed
    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
}

void whenReleasedOnMe()
{
    button.onRelease(&button, 15, 15, 48);

    TEST_ASSERT_TRUE(handlerCalled) // THEN handler is called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedOnMeRepeated()
{
    state.eventHandlingState = EHS_REPEAT;
    button.onRelease(&button, 15, 15, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedOnMeDisabled()
{
    state.mode = BM_DISABLED;
    button.onRelease(&button, 15, 15, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedOnMeRepeatedDisabled()
{
    state.eventHandlingState = EHS_REPEAT;
    state.mode = BM_DISABLED;
    button.onRelease(&button, 15, 15, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_REPEAT); // THEN state is not changed
}

void whenReleasedNotOnMe()
{
    button.onRelease(&button, 115, 115, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedNotOnMeRepeated()
{
    state.eventHandlingState = EHS_REPEAT;
    button.onRelease(&button, 115, 115, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTouch);
    RUN_TEST(whenTouchDisabled);
    RUN_TEST(whenReleasedOnMe);
    RUN_TEST(whenReleasedOnMeRepeated);
    RUN_TEST(whenReleasedOnMeDisabled);
    RUN_TEST(whenReleasedOnMeRepeatedDisabled);
    RUN_TEST(whenReleasedNotOnMe);
    RUN_TEST(whenReleasedNotOnMeRepeated);
    UNITY_END();
}