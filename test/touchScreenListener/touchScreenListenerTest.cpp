#include <unity.h>

#include "../watchMock.hpp"

#include "task/touchScreenListener.cpp"

bool getTouchResult;
signed short xResult;
signed short yResult;

WatchApi watchApi;

TouchScreenListenerParameters p;

bool getTouchStub(signed short &x, signed short &y)
{
    x = xResult;
    y = yResult;
    return getTouchResult;
}

void setUp(void)
{
    watchApi = watchApiMock();
    watchApi.getTouch = getTouchStub;

    p = {
        .touched = false,
        .firstX = 0,
        .firstY = 0,
        .lastX = 0,
        .lastY = 0,
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

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenFirstTouch);
    UNITY_END();
}