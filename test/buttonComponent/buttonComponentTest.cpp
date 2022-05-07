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
    button.onTouch(&button, 15, 15, 48);

    button.onRelease(&button, 15, 15, 2048);

    TEST_ASSERT_TRUE(handlerCalled) // THEN handler is called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedOnMeRepeated()
{
    state.eventGenerate = EG_REPEAT;
    button.onTouch(&button, 15, 15, 48);
    button.onMove(&button, 15, 15, 2048);
    handlerCalled = false; // need to reset because it is set to true when first repeat

    button.onRelease(&button, 15, 15, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
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
    button.onTouch(&button, 15, 15, 48);
    button.onMove(&button, 15, 15, 2048);
    handlerCalled = false; // need to reset because it is set to true when first repeat
    state.mode = BM_DISABLED;

    button.onRelease(&button, 15, 15, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedNotOnMe()
{
    button.onRelease(&button, 115, 115, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedNotOnMeRepeated()
{
    state.eventGenerate = EG_REPEAT;
    button.onTouch(&button, 15, 15, 48);
    button.onMove(&button, 15, 15, 2048);
    handlerCalled = false; // need to reset because it is set to true when first repeat

    button.onRelease(&button, 115, 115, 48);

    TEST_ASSERT_FALSE(handlerCalled) // THEN handler is not called
    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenMoveOutside()
{
    button.onTouch(&button, 15, 15, 48);

    button.onMove(&button, 115, 115, 148);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_LEAVE); // THEN state is set to leave
}

void whenMoveOutsideDisabled()
{
    button.onTouch(&button, 15, 15, 48);

    state.mode = BM_DISABLED;
    button.onMove(&button, 115, 115, 148);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenMoveInside()
{
    button.onTouch(&button, 15, 15, 48);

    button.onMove(&button, 16, 16, 148);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_PRESS); // THEN state is set to press
}

void whenMoveInsideDisabled()
{
    button.onTouch(&button, 15, 15, 48);

    state.mode = BM_DISABLED;
    button.onMove(&button, 16, 16, 148);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenMoveInsideRepeatedBeforeDelay()
{
    state.eventGenerate = EG_REPEAT;
    button.onTouch(&button, 15, 15, 48);
    button.onMove(&button, 16, 16, 148);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_PRESS); // THEN state is still press
    TEST_ASSERT_EQUAL_UINT32(state.lastRepeatTick, 0); // THEN first touch tick is not changed
    TEST_ASSERT_FALSE(handlerCalled); // THEN no handler is called
}

void whenMoveInsideRepeatedAfterDelay()
{
    state.eventGenerate = EG_REPEAT;
    button.onTouch(&button, 15, 15, 48);
    button.onMove(&button, 16, 16, 2048);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_REPEAT); // THEN state is changed to repeat
    TEST_ASSERT_EQUAL_UINT32(state.lastRepeatTick, 2048); // THEN first touch tick is not changed
    TEST_ASSERT_TRUE(handlerCalled); // THEN handler is called
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
    RUN_TEST(whenMoveOutside);
    RUN_TEST(whenMoveOutsideDisabled);
    RUN_TEST(whenMoveInside);
    RUN_TEST(whenMoveInsideDisabled);
    RUN_TEST(whenMoveInsideRepeatedBeforeDelay);
    RUN_TEST(whenMoveInsideRepeatedAfterDelay);
    UNITY_END();
}