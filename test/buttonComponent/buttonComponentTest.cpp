#include <unity.h>

#include "../systemMock.hpp"

#include "core/component/factory.cpp"
#include "core/component/group.cpp"
#include "core/component/component.cpp"
#include "core/component/textComponent.cpp"
#include "core/component/buttonComponent.cpp"
#include "core/component/stepperComponent.cpp"

char TEST[] = "test";
bool handlerCalled;
Factory factory;
ButtonComponentState *state;
Component *button;

unsigned int firstTouchTick;
unsigned int secondTouchTick;
unsigned int thirdTouchTick;
unsigned int touchBeforeFirstDelay;
unsigned int touchAfterFirstDelay;
unsigned int touchBeforeRepeatDelay;
unsigned int touchAfterRepeatDelay;

void handlerStub(void *context)
{
    handlerCalled = true;
}

void setUp(void)
{
    handlerCalled = false;

    SystemApi systemApi = systemApiMock();
    factory = createFactory(&systemApi);

    state = factory.createButtonStateRef(TEST, EG_ONCE, handlerStub);
    button = factory.createButtonComponentRef(10, 10, 10, 10, state);

    firstTouchTick = 48;
    secondTouchTick = firstTouchTick + 10;
    thirdTouchTick = secondTouchTick + 10;
    touchBeforeFirstDelay = firstTouchTick + state->delayTick - 100;
    touchAfterFirstDelay = firstTouchTick + state->delayTick + 100;
    touchBeforeRepeatDelay = touchAfterFirstDelay + state->repeatTick - 50;
    touchAfterRepeatDelay = touchAfterFirstDelay + state->repeatTick + 50;
}

void whenTouch()
{
    button->onTouch(button, 15, 15, firstTouchTick);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_PRESS); // THEN state is set to press
    TEST_ASSERT_EQUAL_UINT32(state->firstTouchTick, firstTouchTick); // THEN first touch tick is stored
    TEST_ASSERT_FALSE(handlerCalled); // THEN handler is not called
}

void whenTouchDisabled()
{
    state->mode = BM_DISABLED;

    button->onTouch(button, 15, 15, firstTouchTick);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is not changed
    TEST_ASSERT_EQUAL_UINT32(state->firstTouchTick, 0); // THEN first touch tick is not changed
    TEST_ASSERT_FALSE(handlerCalled); // THEN handler is not called
}

void whenReleasedOnMe()
{
    button->onTouch(button, 15, 15, firstTouchTick);

    button->onRelease(button, 15, 15, secondTouchTick);

    TEST_ASSERT_TRUE(handlerCalled); // THEN handler is called
    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedOnMeRepeated()
{
    state->eventGenerate = EG_REPEAT;
    button->onTouch(button, 15, 15, firstTouchTick);
    button->onMove(button, 15, 15, touchAfterFirstDelay);
    handlerCalled = false; // need to reset because it is set to true when first repeat

    button->onRelease(button, 15, 15, touchAfterRepeatDelay);

    TEST_ASSERT_FALSE(handlerCalled); // THEN handler is not called
    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedOnMeDisabled()
{
    state->mode = BM_DISABLED;

    button->onRelease(button, 15, 15, firstTouchTick);

    TEST_ASSERT_FALSE(handlerCalled); // THEN handler is not called
    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedOnMeRepeatedDisabled()
{
    button->onTouch(button, 15, 15, firstTouchTick);
    button->onMove(button, 15, 15, touchAfterFirstDelay);
    handlerCalled = false; // need to reset because it is set to true when first repeat
    state->mode = BM_DISABLED;

    button->onRelease(button, 15, 15, touchAfterRepeatDelay);

    TEST_ASSERT_FALSE(handlerCalled); // THEN handler is not called
    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedNotOnMe()
{
    button->onRelease(button, 115, 115, firstTouchTick);

    TEST_ASSERT_FALSE(handlerCalled); // THEN handler is not called
    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenReleasedNotOnMeRepeated()
{
    state->eventGenerate = EG_REPEAT;
    button->onTouch(button, 15, 15, firstTouchTick);
    button->onMove(button, 15, 15, touchAfterFirstDelay);
    handlerCalled = false; // need to reset because it is set to true when first repeat

    button->onRelease(button, 115, 115, touchAfterRepeatDelay);

    TEST_ASSERT_FALSE(handlerCalled); // THEN handler is not called
    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenMoveOutside()
{
    button->onTouch(button, 15, 15, firstTouchTick);

    button->onMove(button, 115, 115, secondTouchTick);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_LEAVE); // THEN state is set to leave
}

void whenMoveOutsideDisabled()
{
    button->onTouch(button, 15, 15, firstTouchTick);

    state->mode = BM_DISABLED;
    button->onMove(button, 115, 115, secondTouchTick);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenMoveInside()
{
    button->onTouch(button, 15, 15, firstTouchTick);

    button->onMove(button, 16, 16, secondTouchTick);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_PRESS); // THEN state is set to press
}

void whenMoveInsideDisabled()
{
    button->onTouch(button, 15, 15, firstTouchTick);

    state->mode = BM_DISABLED;
    button->onMove(button, 16, 16, secondTouchTick);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_IDLE); // THEN state is set to idle
}

void whenMoveInsideRepeatedBeforeDelay()
{
    state->eventGenerate = EG_REPEAT;
    button->onTouch(button, 15, 15, firstTouchTick);
    button->onMove(button, 16, 16, touchBeforeFirstDelay);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_PRESS); // THEN state is still press
    TEST_ASSERT_EQUAL_UINT32(state->lastRepeatTick, 0); // THEN last repeat tick is not changed
    TEST_ASSERT_FALSE(handlerCalled); // THEN no handler is called
}

void whenMoveInsideRepeatedAfterDelay()
{
    state->eventGenerate = EG_REPEAT;
    button->onTouch(button, 15, 15, firstTouchTick);
    button->onMove(button, 16, 16, touchAfterFirstDelay);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_REPEAT); // THEN state is changed to repeat
    TEST_ASSERT_EQUAL_UINT32(state->lastRepeatTick, touchAfterFirstDelay); // THEN last repeat tick is updated
    TEST_ASSERT_TRUE(handlerCalled); // THEN handler is called
}

void whenMoveInsideRepeatedBeforeRepeatDelay()
{
    state->eventGenerate = EG_REPEAT;
    button->onTouch(button, 15, 15, firstTouchTick);
    button->onMove(button, 16, 16, touchAfterFirstDelay);
    handlerCalled = false;
    button->onMove(button, 17, 17, touchBeforeRepeatDelay);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_REPEAT); // THEN state is still repeat
    TEST_ASSERT_EQUAL_UINT32(state->lastRepeatTick, touchAfterFirstDelay); // THEN last repeat tick is not changed
    TEST_ASSERT_FALSE(handlerCalled); // THEN no handler is called
}

void whenMoveInsideRepeatedAfterRepeatDelay()
{
    state->eventGenerate = EG_REPEAT;
    button->onTouch(button, 15, 15, firstTouchTick);
    button->onMove(button, 16, 16, touchAfterFirstDelay);
    handlerCalled = false;
    button->onMove(button, 17, 17, touchAfterRepeatDelay);

    TEST_ASSERT_TRUE(state->eventHandlingState == EHS_REPEAT); // THEN state is still repeat
    TEST_ASSERT_EQUAL_UINT32(state->lastRepeatTick, touchAfterRepeatDelay); // THEN last repeat tick is updated
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
    RUN_TEST(whenMoveInsideRepeatedBeforeRepeatDelay);
    RUN_TEST(whenMoveInsideRepeatedAfterRepeatDelay);
    UNITY_END();
}