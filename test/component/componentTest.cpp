#include <unity.h>

#include "component/component.cpp"

void whenMount() 
{
    Component component = {};
    component.x = 42;
    component.y = 44;

    componentMount(&component, 5 ,6);

    TEST_ASSERT_EQUAL_INT16(47, component.x); // THEN x coordinate adjusted
    TEST_ASSERT_EQUAL_INT16(50, component.y); // THEN y coordinate adjusted
}

void whenPointOnBorder() 
{
    Component component = {};
    component.x = 42;
    component.y = 44;
    component.w = 5;
    component.h = 6;
    
    TEST_ASSERT_EQUAL_UINT64(&component, componentContains(&component, 42, 44)); // THEN pointer to component returns
    TEST_ASSERT_EQUAL_UINT64(&component, componentContains(&component, 42, 50)); // THEN pointer to component returns
    TEST_ASSERT_EQUAL_UINT64(&component, componentContains(&component, 47, 44)); // THEN pointer to component returns
    TEST_ASSERT_EQUAL_UINT64(&component, componentContains(&component, 47, 50)); // THEN pointer to component returns
}

void whenPointInside()
{
    Component component = {};
    component.x = 42;
    component.y = 44;
    component.w = 5;
    component.h = 6;

    TEST_ASSERT_EQUAL_UINT64(&component, componentContains(&component, 44, 47)); // THEN pointer to component returns
}

void whenPointOutside()
{
    Component component = {};
    component.x = 42;
    component.y = 44;
    component.w = 5;
    component.h = 6;

    TEST_ASSERT_EQUAL_UINT64(NULL, componentContains(&component, 52, 54)); // THEN NULL pointer returns
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenMount);
    RUN_TEST(whenPointOnBorder);
    RUN_TEST(whenPointInside);
    RUN_TEST(whenPointOutside);
    UNITY_END();
}