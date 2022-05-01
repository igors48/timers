#include <unity.h>

#include "../watchMock.hpp"
#include "../systemMock.hpp"

#include "core/task/touchScreenListener.cpp"

int watchMutex;
long lastUserEventTimestamp;
bool getTouchResult;
signed short xResult;
signed short yResult;
signed short xTouch;
signed short yTouch;
long timeResult;
Component component;
bool onTouchHandlerCalled;
signed short onTouchHandlerX;
signed short onTouchHandlerY;
bool onMoveHandlerCalled;
signed short onMoveHandlerX;
signed short onMoveHandlerY;
bool onReleaseHandlerCalled;
signed short onReleaseHandlerX;
signed short onReleaseHandlerY;
Tiler tiler;
bool screenOnGestureCalled;
Gesture screenGesture;

WatchApi watchApi;
SystemApi systemApi;

TouchScreenListenerParameters p;

bool getTouchStub(signed short &x, signed short &y)
{
    x = xResult;
    y = yResult;
    return getTouchResult;
}

Component *noTargetStub(signed short x, signed short y)
{
    return NULL;
}

Component *findTargetStub(signed short x, signed short y)
{
    return &component;
}

long timeStub()
{
    return timeResult;
}

void componentOnTouchStub(Component *component, signed short x, signed short y)
{
    onTouchHandlerCalled = true;
    onTouchHandlerX = x;
    onTouchHandlerY = y;
}

void componentOnMoveStub(Component *component, signed short x, signed short y)
{
    onMoveHandlerCalled = true;
    onMoveHandlerX = x;
    onMoveHandlerY = y;
}

void componentOnReleaseStub(Component *component, signed short x, signed short y)
{
    onReleaseHandlerCalled = true;
    onReleaseHandlerX = x;
    onReleaseHandlerY = y;
}

void screenOnGestureSub(Component *component, Gesture gesture)
{
    screenOnGestureCalled = true;
    screenGesture = gesture;
}

void setUp(void)
{
    timeResult = 42;

    xTouch = 0;
    yTouch = 0;

    lastUserEventTimestamp = 0;

    systemApi = systemApiMock();
    systemApi.time = timeStub;

    watchApi = watchApiMock();
    watchApi.getTouch = getTouchStub;

    onTouchHandlerCalled = false;
    onTouchHandlerX = 0;
    onTouchHandlerY = 0;
    onMoveHandlerCalled = false;
    onMoveHandlerX = 0;
    onMoveHandlerY = 0;
    onReleaseHandlerCalled = false;
    onReleaseHandlerX = 0;
    onReleaseHandlerY = 0;
    component = {};
    component.onTouch = componentOnTouchStub;
    component.onMove = componentOnMoveStub;
    component.onRelease = componentOnReleaseStub;

    tiler = {};
    tiler.contains = noTargetStub;
    tiler.onGesture = screenOnGestureSub;
    tiler.onTouch = componentOnTouchStub;
    tiler.onMove = componentOnMoveStub; 
    tiler.onRelease = componentOnReleaseStub; 

    screenOnGestureCalled = false;
    screenGesture = NONE;

    p = {
        .target = NULL,
        .firstX = -1,
        .firstY = -1,
        .lastX = 0,
        .lastY = 0,
        .watchMutex = &watchMutex,
        .lastUserEventTimestamp = &lastUserEventTimestamp,
        .tiler = &tiler,
        .watchApi = &watchApi,
        .systemApi = &systemApi,
    };
}

void whenFirstTouchOutsideComponent()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;

    touchScreenListener(&p);

    TEST_ASSERT_NULL(p.target);                        // THEN target is NULL
    TEST_ASSERT_EQUAL_CHAR(0, onTouchHandlerCalled);   // THEN component onTouch handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onMoveHandlerCalled);    // THEN component onMove handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onReleaseHandlerCalled); // THEN component onRelease handler not called
    TEST_ASSERT_EQUAL_INT16(48, p.lastX);              // THEN last coordinates updated
    TEST_ASSERT_EQUAL_INT16(49, p.lastY);
    TEST_ASSERT_EQUAL_INT64(42, lastUserEventTimestamp); //THEN last user event timestamp updated
    TEST_ASSERT_FALSE(screenOnGestureCalled); // THEN no gesture detected
}

void whenFirstTouchInsideComponent()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;
    tiler.contains = findTargetStub;

    touchScreenListener(&p);

    TEST_ASSERT_NOT_NULL(p.target);                  // THEN target is set to found component
    TEST_ASSERT_EQUAL_CHAR(1, onTouchHandlerCalled); // THEN component onTouch handler called
    TEST_ASSERT_EQUAL_INT16(48, onTouchHandlerX);    // THEN last coordinates passed to handler
    TEST_ASSERT_EQUAL_INT16(49, onTouchHandlerY);
    TEST_ASSERT_EQUAL_CHAR(0, onMoveHandlerCalled);    // THEN component onMove handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onReleaseHandlerCalled); // THEN component onRelease handler not called
    TEST_ASSERT_EQUAL_INT16(48, p.lastX);              // THEN last coordinates updated
    TEST_ASSERT_EQUAL_INT16(49, p.lastY);
    TEST_ASSERT_EQUAL_INT16(48, p.firstX);              // THEN first coordinates updated
    TEST_ASSERT_EQUAL_INT16(49, p.firstY);
    TEST_ASSERT_EQUAL_INT64(42, lastUserEventTimestamp); //THEN last user event timestamp updated
    TEST_ASSERT_FALSE(screenOnGestureCalled); // THEN no gesture detected
}

void whenNotFirstTouch()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;
    tiler.contains = findTargetStub;

    touchScreenListener(&p);

    xResult = 50;
    yResult = 51;
    timeResult = 43;
    onTouchHandlerCalled = 0;

    touchScreenListener(&p);

    TEST_ASSERT_NOT_NULL(p.target);                  // THEN target is set to found component
    TEST_ASSERT_EQUAL_CHAR(0, onTouchHandlerCalled); // THEN component onTouch handler not called
    TEST_ASSERT_EQUAL_CHAR(1, onMoveHandlerCalled);  // THEN component onMove handler called
    TEST_ASSERT_EQUAL_INT16(50, onMoveHandlerX);     // THEN last coordinates passed to handler
    TEST_ASSERT_EQUAL_INT16(51, onMoveHandlerY);
    TEST_ASSERT_EQUAL_CHAR(0, onReleaseHandlerCalled);   // THEN component onRelease handler not called
    TEST_ASSERT_EQUAL_INT16(50, p.lastX);                // THEN last coordinates updated
    TEST_ASSERT_EQUAL_INT16(51, p.lastY);                // THEN last coordinates updated
    TEST_ASSERT_EQUAL_INT64(43, lastUserEventTimestamp); //THEN last user event timestamp updated
    TEST_ASSERT_FALSE(screenOnGestureCalled); // THEN no gesture detected
}

void whenNotFirstTouchWithoutTarget()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;

    touchScreenListener(&p);

    xResult = 50;
    yResult = 51;
    timeResult = 43;
    onTouchHandlerCalled = 0;

    touchScreenListener(&p);

    TEST_ASSERT_NULL(p.target);                  // THEN target is set to found component
    TEST_ASSERT_EQUAL_CHAR(0, onTouchHandlerCalled); // THEN component onTouch handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onMoveHandlerCalled);  // THEN component onMove handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onReleaseHandlerCalled);   // THEN component onRelease handler not called
    TEST_ASSERT_EQUAL_INT16(50, p.lastX);                // THEN last coordinates updated
    TEST_ASSERT_EQUAL_INT16(51, p.lastY);                // THEN last coordinates updated
    TEST_ASSERT_EQUAL_INT64(43, lastUserEventTimestamp); //THEN last user event timestamp updated
    TEST_ASSERT_FALSE(screenOnGestureCalled); // THEN no gesture detected
}

void whenReleasedWithoutTouchComponentBefore()
{
    getTouchResult = false;

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_CHAR(0, onTouchHandlerCalled);   // THEN component onTouch handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onMoveHandlerCalled);    // THEN component onMove handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onReleaseHandlerCalled); // THEN component onRelease handler not called
    TEST_ASSERT_EQUAL_INT64(0, lastUserEventTimestamp); //THEN last user event timestamp not changed
    TEST_ASSERT_FALSE(screenOnGestureCalled); // THEN no gesture detected
}

void whenReleasedWithTouchComponentBefore()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;
    tiler.contains = findTargetStub;

    touchScreenListener(&p);

    xResult = 50;
    yResult = 51;
    onTouchHandlerCalled = 0;

    touchScreenListener(&p);

    getTouchResult = false;
    onMoveHandlerCalled = 0;
    timeResult = 43;

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_CHAR(0, onTouchHandlerCalled);   // THEN component onTouch handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onMoveHandlerCalled);    // THEN component onMove handler not called
    TEST_ASSERT_EQUAL_CHAR(1, onReleaseHandlerCalled); // THEN component onRelease handler not called
    TEST_ASSERT_EQUAL_INT16(50, onReleaseHandlerX);     // THEN last coordinates passed to handler
    TEST_ASSERT_EQUAL_INT16(51, onReleaseHandlerY);
    TEST_ASSERT_EQUAL_INT64(43, lastUserEventTimestamp); //THEN last user event timestamp updated
    TEST_ASSERT_FALSE(screenOnGestureCalled); // THEN no gesture detected (diff is small)
}

void testGestureDetection()
{
    TEST_ASSERT_TRUE(detectGesture(10, 120, 220, 120) == MOVE_RIGHT);
    TEST_ASSERT_TRUE(detectGesture(220, 120, 10, 120) == MOVE_LEFT);
    TEST_ASSERT_TRUE(detectGesture(120, 10, 120, 220) == MOVE_DOWN);
    TEST_ASSERT_TRUE(detectGesture(120, 220, 120, 10) == MOVE_UP);
    TEST_ASSERT_TRUE(detectGesture(120, 120, 100, 130) == NONE);
}

void whenReleasedWithoutTouchComponentBeforeButWithGesture()
{
    xResult = 48;
    yResult = 9;
    getTouchResult = true;

    touchScreenListener(&p);

    xResult = 50;
    yResult = 209;

    touchScreenListener(&p);

    getTouchResult = false;
    timeResult = 43;

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_CHAR(0, onTouchHandlerCalled);   // THEN component onTouch handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onMoveHandlerCalled);    // THEN component onMove handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onReleaseHandlerCalled); // THEN component onRelease handler not called
    TEST_ASSERT_EQUAL_INT64(43, lastUserEventTimestamp); //THEN last user event timestamp updated
    TEST_ASSERT_TRUE(screenOnGestureCalled); // THEN gesture detected
    TEST_ASSERT_TRUE(screenGesture == MOVE_DOWN); // THEN gesture detected
}

void whenReleasedWithTouchComponentBeforeButWithGesture()
{
    xResult = 48;
    yResult = 9;
    getTouchResult = true;
    tiler.contains = findTargetStub;

    touchScreenListener(&p);

    xResult = 50;
    yResult = 209;

    touchScreenListener(&p);
    
    getTouchResult = false;
    onTouchHandlerCalled = 0;
    onMoveHandlerCalled = 0;
    timeResult = 43;

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_CHAR(0, onTouchHandlerCalled);   // THEN component onTouch handler not called
    TEST_ASSERT_EQUAL_CHAR(0, onMoveHandlerCalled);    // THEN component onMove handler not called
    TEST_ASSERT_EQUAL_CHAR(1, onReleaseHandlerCalled); // THEN component onRelease handler not called
    TEST_ASSERT_EQUAL_INT16(50, onReleaseHandlerX);     // THEN last coordinates passed to handler
    TEST_ASSERT_EQUAL_INT16(209, onReleaseHandlerY);
    TEST_ASSERT_EQUAL_INT64(43, lastUserEventTimestamp); //THEN last user event timestamp not changed
    TEST_ASSERT_TRUE(screenOnGestureCalled); // THEN gesture detected
    TEST_ASSERT_TRUE(screenGesture == MOVE_DOWN); // THEN gesture detected
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenFirstTouchOutsideComponent);
    RUN_TEST(whenFirstTouchInsideComponent);
    RUN_TEST(whenNotFirstTouch);
    RUN_TEST(whenNotFirstTouchWithoutTarget);
    RUN_TEST(whenReleasedWithoutTouchComponentBefore);
    RUN_TEST(whenReleasedWithTouchComponentBefore);
    RUN_TEST(testGestureDetection);
    RUN_TEST(whenReleasedWithoutTouchComponentBeforeButWithGesture);
    RUN_TEST(whenReleasedWithTouchComponentBeforeButWithGesture);
    UNITY_END();
}