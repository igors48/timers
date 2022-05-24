#include <unity.h>

#include "core/app/tiler.cpp"

TftApi tftApiStub;
App appStub;
Component activeTileStub;
Component child;
bool fillRectCalled;
bool activeTileRendered;
bool activeTileRenderMode;
bool activeTileContainsCalled;
bool activeTileOnGestureCalled;
bool activeTileOnButtonCalled;
bool activeTileStateUpdated;
bool childOnTouchCalled;
bool childOnMoveCalled;
bool childOnReleaseCalled;
Component *eventTarget;

Tiler tiler;

Component *getActiveTile()
{
    return &activeTileStub;
}

void fillRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    fillRectCalled = true;
}

void drawFastVLine(signed int x, signed int y, signed int h, unsigned int color)
{
    // empty
}

void drawFastHLine(signed int x, signed int y, signed int w, unsigned int color)
{
    // empty
}

void render(Component *component, bool forced, TftApi *tftApi)
{
    activeTileRendered = true;
    activeTileRenderMode = forced;
}

void updateState(Component *component)
{
    activeTileStateUpdated = true;
}

Component *contains(Component *component, signed short x, signed short y)
{
    activeTileContainsCalled = true;
    return &child;
}

void onGesture(Component *component, Gesture gesture)
{
    activeTileOnGestureCalled = true;
    eventTarget = component;
}

void onButton(Component *component)
{
    activeTileOnButtonCalled = true;
    eventTarget = component;
}

void childOnTouchStub(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    childOnTouchCalled = true;
    eventTarget = component;
}

void childOnMoveStub(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    childOnMoveCalled = true;
    eventTarget = component;
}

void childOnReleaseStub(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    childOnReleaseCalled = true;
    eventTarget = component;
}

void setUp(void)
{
    activeTileRendered = false;
    activeTileRenderMode = false;
    activeTileContainsCalled = false;
    activeTileOnGestureCalled = false;
    activeTileStateUpdated = false;
    activeTileOnButtonCalled = false;

    childOnTouchCalled = false;
    childOnMoveCalled = false;
    childOnReleaseCalled = false;

    eventTarget = NULL;

    child = {
        .onTouch = childOnTouchStub,
        .onMove = childOnMoveStub,
        .onRelease = childOnReleaseStub,
    };

    activeTileStub = {
        .contains = contains,
        .onGesture = onGesture,
        .onButton = onButton,
        .render = render,
        .updateState = updateState,
    };

    appStub = {
        .getActiveTile = getActiveTile,
    };

    fillRectCalled = false;

    tftApiStub = {
        .fillRect = fillRect,
        .drawFastVLine = drawFastVLine,
        .drawFastHLine = drawFastHLine,
    };

    tiler = createTiler(&tftApiStub);
    tiler.setApp(&appStub);
}

void whenRenderInForcedMode()
{
    tiler.renderApp(true);

    TEST_ASSERT_TRUE(fillRectCalled);         // THEN screen cleared
    TEST_ASSERT_TRUE(activeTileRendered);     // THEN active tile rendered
    TEST_ASSERT_TRUE(activeTileRenderMode);   // THEN active tile rendered in the forced mode
    TEST_ASSERT_TRUE(activeTileStateUpdated); // THEN active tile state updated
}

void whenRenderInNotForcedMode()
{
    tiler.renderApp(false);

    TEST_ASSERT_FALSE(fillRectCalled);        // THEN screen not cleared
    TEST_ASSERT_TRUE(activeTileRendered);     // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode);  // THEN active tile rendered in the not forced mode
    TEST_ASSERT_TRUE(activeTileStateUpdated); // THEN active tile state updated
}

void whenContainsCalled()
{
    tiler.contains(0, 0);

    TEST_ASSERT_TRUE(activeTileContainsCalled); // THEN active tile contains called
}

void whenOnGestureCalled()
{
    tiler.onGesture(MOVE_DOWN);

    TEST_ASSERT_TRUE(activeTileOnGestureCalled);            // THEN active tile gesture handler called
    TEST_ASSERT_TRUE(activeTileRendered);                   // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode);                // THEN active tile rendered in the not forced mode
    TEST_ASSERT_TRUE(activeTileStateUpdated);               // THEN active tile state updated
    TEST_ASSERT_EQUAL_UINT64(eventTarget, &activeTileStub); // THEN event target is active tile
}

void whenOnButtonCalled()
{
    tiler.onButton();

    TEST_ASSERT_TRUE(activeTileOnButtonCalled);             // THEN active tile button handler called
    TEST_ASSERT_TRUE(activeTileRendered);                   // THEN active tile rendered
    TEST_ASSERT_TRUE(activeTileRenderMode);                // THEN active tile rendered in the not forced mode
    TEST_ASSERT_TRUE(activeTileStateUpdated);               // THEN active tile state updated
    TEST_ASSERT_EQUAL_UINT64(eventTarget, &activeTileStub); // THEN event target is active tile
}

void whenOnTouchCalled()
{
    tiler.onTouch(&child, 0, 0, 0);

    TEST_ASSERT_TRUE(childOnTouchCalled);          // THEN child component on touch handler called
    TEST_ASSERT_TRUE(activeTileRendered);          // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode);       // THEN active tile rendered in the not forced mode
    TEST_ASSERT_TRUE(activeTileStateUpdated);      // THEN active tile state updated
    TEST_ASSERT_EQUAL_UINT64(eventTarget, &child); // THEN event target is child component
}

void whenOnMoveCalled()
{
    tiler.onMove(&child, 0, 0, 0);

    TEST_ASSERT_TRUE(childOnMoveCalled);           // THEN child component on move handler called
    TEST_ASSERT_TRUE(activeTileRendered);          // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode);       // THEN active tile rendered in the not forced mode
    TEST_ASSERT_TRUE(activeTileStateUpdated);      // THEN active tile state updated
    TEST_ASSERT_EQUAL_UINT64(eventTarget, &child); // THEN event target is child component
}

void whenOnReleaseCalled()
{
    tiler.onRelease(&child, 0, 0, 0);

    TEST_ASSERT_TRUE(childOnReleaseCalled);        // THEN child component on release handler called
    TEST_ASSERT_TRUE(activeTileRendered);          // THEN active tile rendered
    TEST_ASSERT_FALSE(activeTileRenderMode);       // THEN active tile rendered in the not forced mode
    TEST_ASSERT_TRUE(activeTileStateUpdated);      // THEN active tile state updated
    TEST_ASSERT_EQUAL_UINT64(eventTarget, &child); // THEN event target is child component
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenRenderInForcedMode);
    RUN_TEST(whenRenderInNotForcedMode);
    RUN_TEST(whenContainsCalled);
    RUN_TEST(whenOnGestureCalled);
    RUN_TEST(whenOnButtonCalled);
    RUN_TEST(whenOnTouchCalled);
    RUN_TEST(whenOnMoveCalled);
    RUN_TEST(whenOnReleaseCalled);
    UNITY_END();
}