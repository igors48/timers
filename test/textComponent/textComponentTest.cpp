#include <unity.h>

#include "core/component/textComponent.cpp"
#include "core/component/component.cpp"

Component component;
TextState textState;

bool provideCalled;

void provideStub(TextState *state)
{
    provideCalled = true;
}

void setUp(void)
{
    textState = {};
    strcpy(textState.content, "a");
    strcpy(textState._content, "a");
    textState.size = 1;
    textState._size = 1;
    textState.fontColor = 2;
    textState._fontColor = 2;
    textState.backColor = 3;
    textState._backColor = 3;
    textState.font = 4;
    textState._font = 4;
    textState.provide = provideStub;
    
    component = {};
    component.state = &textState;
    
    watchState = {};
    
    provideCalled = false;
}

void whenNewState()
{
    textComponentNewState(&component);

    TEST_ASSERT_EQUAL_UINT8(1, provideCalled); // THEN provide function called
}

void whenStateNoChanged()
{
    bool changed = textComponentNewState(&component);

    TEST_ASSERT_EQUAL_UINT8(0, changed); // THEN state no changed
}

void whenContentChanged()
{
    strcpy(textState.content, "b");
    bool changed = textComponentNewState(&component);

    TEST_ASSERT_EQUAL_UINT8(1, changed); // THEN state changed
}

void whenSizeChanged()
{
    textState.size = 48;
    bool changed = textComponentNewState(&component);

    TEST_ASSERT_EQUAL_UINT8(1, changed); // THEN state changed
}

void whenFontColorChanged()
{
    textState.fontColor = 48;
    bool changed = textComponentNewState(&component);

    TEST_ASSERT_EQUAL_UINT8(1, changed); // THEN state changed
}

void whenBackColorChanged()
{
    textState.backColor = 48;
    bool changed = textComponentNewState(&component);

    TEST_ASSERT_EQUAL_UINT8(1, changed); // THEN state changed
}

void whenFontChanged()
{
    textState.font = 48;
    bool changed = textComponentNewState(&component);

    TEST_ASSERT_EQUAL_UINT8(1, changed); // THEN state changed
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenNewState);
    RUN_TEST(whenStateNoChanged);
    RUN_TEST(whenContentChanged);
    RUN_TEST(whenSizeChanged);
    RUN_TEST(whenFontColorChanged);
    RUN_TEST(whenBackColorChanged);
    RUN_TEST(whenFontChanged);
    UNITY_END();
}