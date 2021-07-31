#include <unity.h>

#include "../watchMock.hpp"
#include "../systemMock.hpp"

#include "task/touchScreenListener.cpp"

int lastUserEventTimestampMutex;
long lastUserEventTimestamp;
bool getTouchResult;
signed short xResult;
signed short yResult;
signed short xTouch;
signed short yTouch;
long timeResult;

WatchApi watchApi;
SystemApi systemApi;

TouchScreenListenerParameters p;

bool getTouchStub(signed short &x, signed short &y)
{
    x = xResult;
    y = yResult;
    return getTouchResult;
}

void onTouchStub(signed short x, signed short y)
{
    xTouch = x;
    yTouch = y;
}

long timeStub()
{
    return timeResult;
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

    p = {
        .touched = false,
        .firstX = 0,
        .firstY = 0,
        .lastX = 0,
        .lastY = 0,
        .lastUserEventTimestampMutex = &lastUserEventTimestampMutex,
        .lastUserEventTimestamp = &lastUserEventTimestamp,
        .onTouch = onTouchStub,
        .watchApi = &watchApi,
        .systemApi = &systemApi,
    };
}

void whenFirstTouch()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_INT(1, p.touched);  // THEN touched flag raised 
    TEST_ASSERT_EQUAL_INT16(48, p.firstX);  // THEN first and last coordinates are set to the same values 
    TEST_ASSERT_EQUAL_INT16(49, p.firstY);  // THEN first and last coordinates are set to the same values 
    TEST_ASSERT_EQUAL_INT16(48, p.lastX);  // THEN first and last coordinates are set to the same values 
    TEST_ASSERT_EQUAL_INT16(49, p.lastY);  // THEN first and last coordinates are set to the same values 
    TEST_ASSERT_EQUAL_INT64(42, lastUserEventTimestamp); //THEN last user event timestamp updated
}

void whenNotFirstTouch()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;

    touchScreenListener(&p);

    xResult = 50;
    yResult = 51;
    getTouchResult = true;
    timeResult = 52;
    
    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_INT(1, p.touched);  // THEN touched flag still raised 
    TEST_ASSERT_EQUAL_INT16(48, p.firstX);  // THEN first and not changed 
    TEST_ASSERT_EQUAL_INT16(49, p.firstY);  // THEN first and not changed
    TEST_ASSERT_EQUAL_INT16(50, p.lastX);  // THEN last coordinates are updated 
    TEST_ASSERT_EQUAL_INT16(51, p.lastY);  // THEN last coordinates are updated
    TEST_ASSERT_EQUAL_INT64(52, lastUserEventTimestamp); //THEN last user event timestamp updated
}

void whenReleasedAroundTheFirstTouchPoint()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;
    touchScreenListener(&p);

    xResult = 50;
    yResult = 51;
    getTouchResult = true;
    touchScreenListener(&p);

    getTouchResult = false;
    timeResult = 52;
    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_INT(0, p.touched);  // THEN touched flag reset
    TEST_ASSERT_EQUAL_INT16(48, xTouch);  // THEN onTouch handler called with correct x touch coord 
    TEST_ASSERT_EQUAL_INT16(49, yTouch);  // THEN onTouch handler called with correct y touch coord
    TEST_ASSERT_EQUAL_INT64(52, lastUserEventTimestamp); //THEN last user event timestamp updated
}

void whenReleasedFarFromTheFirstTouchPoint()
{
    timeResult = 47;
    xResult = 48;
    yResult = 49;
    getTouchResult = true;
    touchScreenListener(&p);

    timeResult = 149;
    xResult = 150;
    yResult = 151;
    getTouchResult = true;
    touchScreenListener(&p);

    getTouchResult = false;

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_INT(0, p.touched);  // THEN touched flag reset
    TEST_ASSERT_EQUAL_INT16(0, xTouch);  // THEN onTouch handler does not called  
    TEST_ASSERT_EQUAL_INT16(0, yTouch);  // THEN onTouch handler does not called
    TEST_ASSERT_EQUAL_INT64(149, lastUserEventTimestamp); //THEN last user event timestamp not changed sinse second touch
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenFirstTouch);
    RUN_TEST(whenNotFirstTouch);
    RUN_TEST(whenReleasedAroundTheFirstTouchPoint);
    RUN_TEST(whenReleasedFarFromTheFirstTouchPoint);
    UNITY_END();
}