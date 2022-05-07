#include <unity.h>

#include "core/component/buttonComponent.hpp"

static char TEST[] = "test";

void handlerStub()
{

}

void whenTouch()
{
    ButtonComponentState state = createButtonState(TEST, EG_ONCE, handlerStub);
    Component button = createButtonComponent(10, 10, 10, 10, &state);

    button.onTouch(&button, 15, 15, 48);

    TEST_ASSERT_TRUE(state.eventHandlingState == EHS_PRESS); // THEN state set to press
    TEST_ASSERT_EQUAL_UINT32(state.firstTouchTick, 48); // THEN first touch tick is stored
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTouch);
    UNITY_END();
}