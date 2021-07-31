#include <unity.h>

#include "../watchMock.hpp"

#include "task/touchScreenListener.cpp"

bool getTouchResult;
signed short xResult;
signed short yResult;
signed short xTouch;
signed short yTouch;

WatchApi watchApi;

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

void setUp(void)
{
    xTouch = 0;
    yTouch = 0;

    watchApi = watchApiMock();
    watchApi.getTouch = getTouchStub;

    p = {
        .touched = false,
        .firstX = 0,
        .firstY = 0,
        .lastX = 0,
        .lastY = 0,
        .onTouch = onTouchStub,
        .watchApi = &watchApi,
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

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_INT(1, p.touched);  // THEN touched flag still raised 
    TEST_ASSERT_EQUAL_INT16(48, p.firstX);  // THEN first and not changed 
    TEST_ASSERT_EQUAL_INT16(49, p.firstY);  // THEN first and not changed
    TEST_ASSERT_EQUAL_INT16(50, p.lastX);  // THEN last coordinates are updated 
    TEST_ASSERT_EQUAL_INT16(51, p.lastY);  // THEN last coordinates are updated
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

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_INT(0, p.touched);  // THEN touched flag reset
    TEST_ASSERT_EQUAL_INT16(48, xTouch);  // THEN onTouch handler called with correct x touch coord 
    TEST_ASSERT_EQUAL_INT16(49, yTouch);  // THEN onTouch handler called with correct y touch coord
}

void whenReleasedFarFromTheFirstTouchPoint()
{
    xResult = 48;
    yResult = 49;
    getTouchResult = true;
    touchScreenListener(&p);

    xResult = 150;
    yResult = 151;
    getTouchResult = true;
    touchScreenListener(&p);

    getTouchResult = false;

    touchScreenListener(&p);

    TEST_ASSERT_EQUAL_INT(0, p.touched);  // THEN touched flag reset
    TEST_ASSERT_EQUAL_INT16(0, xTouch);  // THEN onTouch handler does not called  
    TEST_ASSERT_EQUAL_INT16(0, yTouch);  // THEN onTouch handler does not called
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