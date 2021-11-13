#include <unity.h>

#include "component/component.cpp"
#include "component/group.cpp"

typedef struct
{
    bool rendered;
} ChildState;

const unsigned char CHILDREN_COUNT = 3;
void* children[CHILDREN_COUNT];

Component group;
Component child00;
ChildState child00State;
Component child01;
ChildState child01State;
Component child02;
ChildState child02State;
GroupState groupState;

void childRender(signed short x, signed short y, Component *component, WatchState *watchState, TftApi *tftApi)
{
    ChildState *state = (ChildState *)component->state;
    state->rendered = true;
}

void setUp(void)
{
    child00State = {
        .rendered = false,
    };
    child00 = {};
    child00.x = 10;
    child00.y = 20;
    child00.w = 5;
    child00.h = 5;
    child00.mount = componentMount;
    child00.contains = componentContains;
    child00.state = &child00State;

    child01State = {
        .rendered = false,
    };
    child01 = {};
    child01.x = 30;
    child01.y = 40;
    child01.w = 5;
    child01.h = 5;
    child01.mount = componentMount;
    child01.contains = componentContains;
    child01.state = &child00State;

    child02State = {
        .rendered = false,
    };
    child02 = {};
    child02.x = 50;
    child02.y = 60;
    child02.w = 5;
    child02.h = 5;
    child02.mount = componentMount;
    child02.contains = componentContains;
    child02.state = &child00State;

    children[0] = &child00;
    children[1] = &child01;
    children[2] = &child02;

    groupState = {
        .childrenCount = CHILDREN_COUNT,
        .children = children,
    };

    group = createGroupComponent(5, 6, &groupState); 
}

void whenMount() 
{
    groupMount(&group, 12, 13);

    TEST_ASSERT_EQUAL_INT16(10 + 5 + 12, child00.x); // THEN child x coordinate adjusted
    TEST_ASSERT_EQUAL_INT16(20 + 6 + 13, child00.y); // THEN child y coordinate adjusted

    TEST_ASSERT_EQUAL_INT16(30 + 5 + 12, child01.x); // THEN child x coordinate adjusted
    TEST_ASSERT_EQUAL_INT16(40 + 6 + 13, child01.y); // THEN child y coordinate adjusted

    TEST_ASSERT_EQUAL_INT16(50 + 5 + 12, child02.x); // THEN child x coordinate adjusted
    TEST_ASSERT_EQUAL_INT16(60 + 6 + 13, child02.y); // THEN child y coordinate adjusted
}

void whenContains()
{
    groupMount(&group, 12, 13);
    TEST_ASSERT_EQUAL_UINT64(&child00, groupContains(&group, 28, 40)); // THEN pointer to child returns
}

void whenNotContains()
{
    groupMount(&group, 112, 113);
    TEST_ASSERT_EQUAL_UINT64(NULL, groupContains(&group, 28, 40)); // THEN NULL returns
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenMount);
    RUN_TEST(whenContains);
    RUN_TEST(whenNotContains);
    UNITY_END();
}