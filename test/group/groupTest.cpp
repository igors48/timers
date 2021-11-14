#include <unity.h>

#include "component/component.cpp"
#include "component/group.cpp"

typedef struct
{
    bool rendered;
    bool updated;
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

void childRenderStub(Component *component, WatchState *watchState, TftApi *tftApi)
{
    ChildState *state = (ChildState *)component->state;
    state->rendered = true;
}

bool childNewStateTrueStub(Component *component, WatchState *watchState)
{
    ChildState *state = (ChildState *)component->state;
    state->updated = true;
    return true;
}

bool childNewStateFalseStub(Component *component, WatchState *watchState)
{
    ChildState *state = (ChildState *)component->state;
    state->updated = true;
    return false;
}

void setUp(void)
{
    child00State = {
        .rendered = false,
        .updated = false,
    };
    child00 = {};
    child00.x = 10;
    child00.y = 20;
    child00.w = 5;
    child00.h = 5;
    child00.mount = componentMount;
    child00.contains = componentContains;
    child00.render = childRenderStub;
    child00.newState = childNewStateTrueStub;
    child00.state = &child00State;

    child01State = {
        .rendered = false,
        .updated = false,
    };
    child01 = {};
    child01.x = 30;
    child01.y = 40;
    child01.w = 5;
    child01.h = 5;
    child01.mount = componentMount;
    child01.contains = componentContains;
    child01.render = childRenderStub;
    child01.newState = childNewStateTrueStub;
    child01.state = &child01State;

    child02State = {
        .rendered = false,
        .updated = false,
    };
    child02 = {};
    child02.x = 50;
    child02.y = 60;
    child02.w = 5;
    child02.h = 5;
    child02.mount = componentMount;
    child02.contains = componentContains;
    child02.render = childRenderStub;
    child02.newState = childNewStateTrueStub;
    child02.state = &child02State;

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

void whenRender()
{
    groupRender(&group, NULL, NULL);

    TEST_ASSERT_EQUAL_UINT8(1, child00State.rendered); // THEN all children rendered
    TEST_ASSERT_EQUAL_UINT8(1, child01State.rendered); // THEN all children rendered
    TEST_ASSERT_EQUAL_UINT8(1, child02State.rendered); // THEN all children rendered
}

void whenNewState()
{
    bool newState = groupNewState(&group, NULL);

    TEST_ASSERT_EQUAL_UINT8(1, newState); // THEN true as a result
    TEST_ASSERT_EQUAL_UINT8(1, child00State.updated); // THEN all children rendered
    TEST_ASSERT_EQUAL_UINT8(1, child01State.updated); // THEN all children rendered
    TEST_ASSERT_EQUAL_UINT8(1, child02State.updated); // THEN all children rendered
}

void whenAllChildrenNewStateReturnedFalse()
{
    child00.newState = childNewStateFalseStub;
    child01.newState = childNewStateFalseStub;
    child02.newState = childNewStateFalseStub;

    bool newState = groupNewState(&group, NULL);

    TEST_ASSERT_EQUAL_UINT8(0, newState); // THEN false as a result
    TEST_ASSERT_EQUAL_UINT8(1, child00State.updated); // THEN all children rendered
    TEST_ASSERT_EQUAL_UINT8(1, child01State.updated); // THEN all children rendered
    TEST_ASSERT_EQUAL_UINT8(1, child02State.updated); // THEN all children rendered
}

void whenNotAllChildrenNewStateReturnedFalse()
{
    child00.newState = childNewStateFalseStub;
    child02.newState = childNewStateFalseStub;

    bool newState = groupNewState(&group, NULL);

    TEST_ASSERT_EQUAL_UINT8(1, newState); // THEN true as a result
    TEST_ASSERT_EQUAL_UINT8(1, child00State.updated); // THEN all children rendered
    TEST_ASSERT_EQUAL_UINT8(1, child01State.updated); // THEN all children rendered
    TEST_ASSERT_EQUAL_UINT8(1, child02State.updated); // THEN all children rendered
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenMount);
    RUN_TEST(whenContains);
    RUN_TEST(whenNotContains);
    RUN_TEST(whenRender);
    RUN_TEST(whenNewState);
    RUN_TEST(whenAllChildrenNewStateReturnedFalse);
    RUN_TEST(whenNotAllChildrenNewStateReturnedFalse);
    UNITY_END();
}