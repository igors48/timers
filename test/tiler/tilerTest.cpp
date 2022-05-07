#include <unity.h>

#include "core/app/tiler.cpp"

TftApi tftApiStub;
App appStub;
Component activeTileStub;
bool fillRectCalled;
bool activeTileRendered;
bool activeTileRenderMode;
bool activeTileContainsCalled;
bool activeTileOnGestureCalled;
bool onTouchCalled;
bool onMoveCalled;
bool onReleaseCalled;

Tiler tiler;

Component* getActiveTile()
{
    return &activeTileStub;
}

void fillRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    fillRectCalled = true;
}

void render(Component *component, bool forced, TftApi *tftApi)
{
    activeTileRendered = true;
    activeTileRenderMode = forced;
}

Component* contains(Component *component, signed short x, signed short y)
{
    activeTileContainsCalled = true;
    return &activeTileStub;
}

void onGesture(Component *component, Gesture gesture)
{
    activeTileOnGestureCalled = true;
}

void onTouchStub(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    onTouchCalled = true;
}

void onMoveStub(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    onMoveCalled = true;
}

void onReleaseStub(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    onReleaseCalled = true;
}

void setUp(void)
{
    activeTileRendered = false;
    activeTileRenderMode = false;
    activeTileContainsCalled = false;
    activeTileOnGestureCalled = false;

    onTouchCalled = false;
    onMoveCalled = false;
    onReleaseCalled = false;

    activeTileStub = {
        .contains = contains,
        .onTouch = onTouchStub,
        .onMove = onMoveStub,
        .onRelease = onReleaseStub,
        .onGesture = onGesture,
        .render = render,
    };

    appStub = {
        .getActiveTile = getActiveTile,
    };

    fillRectCalled = false;
    
    tftApiStub = {
        .fillRect = fillRect,
    };

    tiler = createTiler(&tftApiStub);
    tiler.setApp(&appStub);
}

void whenRenderInForcedMode()
{    
    tiler.renderApp(true);

    TEST_ASSERT_TRUE(fillRectCalled); // THEN screen cleared
    TEST_ASSERT_TRUE(activeTileRendered); // THEN active tile rendered
    TEST_ASSERT_TRUE(activeTileRenderMode); // THEN active tile rendered in the forced mode
}

void whenRenderInNotForcedMode()
{
    tiler.renderApp(false);

    TEST_ASSERT_FALSE(fillRectCalled); // THEN screen not cleared
    TEST_ASSERT_TRUE(activeTileRendered); // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode); // THEN active tile rendered in the not forced mode
}

void whenContainsCalled()
{
    tiler.contains(0, 0);

    TEST_ASSERT_TRUE(activeTileContainsCalled); // THEN active tile contains called
}

void whenOnGestureCalled()
{
    tiler.onGesture(MOVE_DOWN);

    TEST_ASSERT_TRUE(activeTileOnGestureCalled); // THEN active tile gesture handler called
    TEST_ASSERT_TRUE(activeTileRendered); // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode); // THEN active tile rendered in the not forced mode
}

void whenOnTouchCalled()
{
    tiler.onTouch(&activeTileStub, 0, 0, 0);

    TEST_ASSERT_TRUE(onTouchCalled); // THEN component on touch handler called
    TEST_ASSERT_TRUE(activeTileRendered); // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode); // THEN active tile rendered in the not forced mode
}

void whenOnMoveCalled()
{
    tiler.onMove(&activeTileStub, 0, 0, 0);

    TEST_ASSERT_TRUE(onMoveCalled); // THEN component on move handler called
    TEST_ASSERT_TRUE(activeTileRendered); // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode); // THEN active tile rendered in the not forced mode
}

void whenOnReleaseCalled()
{
    tiler.onRelease(&activeTileStub, 0, 0, 0);

    TEST_ASSERT_TRUE(onReleaseCalled); // THEN component on release handler called
    TEST_ASSERT_TRUE(activeTileRendered); // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode); // THEN active tile rendered in the not forced mode
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenRenderInForcedMode);
    RUN_TEST(whenRenderInNotForcedMode);
    RUN_TEST(whenContainsCalled);
    RUN_TEST(whenOnGestureCalled);
    RUN_TEST(whenOnTouchCalled);
    RUN_TEST(whenOnMoveCalled);
    RUN_TEST(whenOnReleaseCalled);
    UNITY_END();
}