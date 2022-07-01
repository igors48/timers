#include <unity.h>

#include "../systemMock.hpp"

#include "core/component/factory.cpp"
#include "core/component/group.cpp"
#include "core/component/component.cpp"
#include "core/component/textComponent.cpp"
#include "core/component/buttonComponent.cpp"
#include "core/component/stepperComponent.cpp"

typedef struct
{
    bool rendered;
    bool updated;
    unsigned long tick;
} ChildState;

const unsigned char CHILDREN_COUNT = 3;
void* children[CHILDREN_COUNT];

Factory factory;

Component *group;
GroupState *groupState;

Component child00;
ChildState child00State;
Component child01;
ChildState child01State;
Component child02;
ChildState child02State;

void childRenderStub(Component *component, bool forced, TftApi *tftApi)
{
    ChildState *state = (ChildState *)component->state;
    state->rendered = true;
}

bool childIsStateModifiedTrueStub(Component *component)
{
    return true;
}

bool childIsStateModifiedFalseStub(Component *component)
{
    return false;
}

void childUpdateStateStub(Component *component)
{
    ChildState *state = (ChildState *)component->state;
    state->updated = true;
}

void childOnTickStub(Component *component, unsigned long tick)
{
    ChildState *state = (ChildState *)component->state;
    state->tick = tick;
}

void setUp(void)
{
    SystemApi systemApi = systemApiMock();
    factory = createFactory(&systemApi);

    child00State = {
        .rendered = false,
        .updated = false,
        .tick = 0,
    };
    child00 = {};
    child00.x = 10;
    child00.y = 20;
    child00.w = 5;
    child00.h = 5;
    child00.mount = componentMount;
    child00.contains = componentContains;
    child00.render = childRenderStub;
    child00.isStateModified = childIsStateModifiedTrueStub;
    child00.updateState = childUpdateStateStub;
    child00.onTick = childOnTickStub;
    child00.state = &child00State;

    child01State = {
        .rendered = false,
        .updated = false,
        .tick = 0,
    };
    child01 = {};
    child01.x = 30;
    child01.y = 40;
    child01.w = 5;
    child01.h = 5;
    child01.mount = componentMount;
    child01.contains = componentContains;
    child01.render = childRenderStub;
    child01.isStateModified = childIsStateModifiedTrueStub;
    child01.updateState = childUpdateStateStub;
    child01.onTick = childOnTickStub;
    child01.state = &child01State;

    child02State = {
        .rendered = false,
        .updated = false,
        .tick = 0,
    };
    child02 = {};
    child02.x = 50;
    child02.y = 60;
    child02.w = 5;
    child02.h = 5;
    child02.mount = componentMount;
    child02.contains = componentContains;
    child02.render = childRenderStub;
    child02.isStateModified = childIsStateModifiedTrueStub;
    child02.updateState = childUpdateStateStub;
    child02.onTick = childOnTickStub;
    child02.state = &child02State;

    children[0] = &child00;
    children[1] = &child01;
    children[2] = &child02;

    groupState = factory.createGroupStateRef(CHILDREN_COUNT, children);
    group = factory.createGroupComponentRef(5, 6, groupState); 
}

void whenMount() 
{
    groupMount(group, 12, 13);

    TEST_ASSERT_EQUAL_INT16(10 + 5 + 12, child00.x); // THEN child x coordinate adjusted
    TEST_ASSERT_EQUAL_INT16(20 + 6 + 13, child00.y); // THEN child y coordinate adjusted

    TEST_ASSERT_EQUAL_INT16(30 + 5 + 12, child01.x); // THEN child x coordinate adjusted
    TEST_ASSERT_EQUAL_INT16(40 + 6 + 13, child01.y); // THEN child y coordinate adjusted

    TEST_ASSERT_EQUAL_INT16(50 + 5 + 12, child02.x); // THEN child x coordinate adjusted
    TEST_ASSERT_EQUAL_INT16(60 + 6 + 13, child02.y); // THEN child y coordinate adjusted
}

void whenContains()
{
    groupMount(group, 12, 13);

    TEST_ASSERT_EQUAL_UINT64(&child00, groupContains(group, 28, 40)); // THEN pointer to child returns
}

void whenGroupContainsGroupWithChild()
{
    void* childGroupComponents[1 * sizeof(void*)];
    childGroupComponents[0] = &child00;
    GroupState* childGroupState = factory.createGroupStateRef(1, childGroupComponents);
    Component* childGroup = factory.createGroupComponentRef(5, 6, childGroupState);

    void* parentGroupComponents[1 * sizeof(void*)]; // todo do the same 1 * sizeof(void*) in other places
    parentGroupComponents[0] = childGroup;

    GroupState* parentGroupState = factory.createGroupStateRef(1, parentGroupComponents);
    Component* parentGroup = factory.createGroupComponentRef(5, 6, parentGroupState);
    
    signed short mountX = 10;
    signed short mountY = 20;
    signed short xInsideMountedChild = child00.x + childGroup->x + parentGroup->x + mountX + 2;
    signed short yInsideMountedChild = child00.y + childGroup->y + parentGroup->y + mountY + 2;

    groupMount(parentGroup, mountX, mountY); 

    TEST_ASSERT_EQUAL_UINT64(&child00, groupContains(parentGroup, xInsideMountedChild, yInsideMountedChild)); // THEN pointer to a child of a child group returns
}

void whenNotContains()
{
    groupMount(group, 112, 113);

    TEST_ASSERT_EQUAL_UINT64(NULL, groupContains(group, 28, 40)); // THEN NULL returns
}

void whenRender()
{
    groupRender(group, false, NULL);

    TEST_ASSERT_TRUE(child00State.rendered); // THEN all children rendered
    TEST_ASSERT_TRUE(child01State.rendered); // THEN all children rendered
    TEST_ASSERT_TRUE(child02State.rendered); // THEN all children rendered
}

void whenRenderNotChangedChildren()
{
    child00.isStateModified = childIsStateModifiedFalseStub;
    child01.isStateModified = childIsStateModifiedFalseStub;
    child02.isStateModified = childIsStateModifiedFalseStub;

    groupRender(group, false, NULL);

    TEST_ASSERT_FALSE(child00State.rendered); // THEN children not rendered
    TEST_ASSERT_FALSE(child01State.rendered); // THEN children not rendered
    TEST_ASSERT_FALSE(child02State.rendered); // THEN children not rendered
}

void whenRenderNotChangedChildrenInForcedMode()
{
    child00.isStateModified = childIsStateModifiedFalseStub;
    child01.isStateModified = childIsStateModifiedFalseStub;
    child02.isStateModified = childIsStateModifiedFalseStub;

    groupRender(group, true, NULL);

    TEST_ASSERT_TRUE(child00State.rendered); // THEN all children rendered
    TEST_ASSERT_TRUE(child01State.rendered); // THEN all children rendered
    TEST_ASSERT_TRUE(child02State.rendered); // THEN all children rendered
}

void whenNewState()
{
    bool newState = groupIsStateModified(group);

    TEST_ASSERT_TRUE(newState); // THEN true as group always return true
}

void whenAllChildrenNewStateReturnedFalse()
{
    child00.isStateModified = childIsStateModifiedFalseStub;
    child01.isStateModified = childIsStateModifiedFalseStub;
    child02.isStateModified = childIsStateModifiedFalseStub;

    bool newState = groupIsStateModified(group);

    TEST_ASSERT_TRUE(newState); // THEN true as group always return true
}

void whenNotAllChildrenNewStateReturnedFalse()
{
    child00.isStateModified = childIsStateModifiedFalseStub;
    child02.isStateModified = childIsStateModifiedFalseStub;

    bool newState = groupIsStateModified(group);

    TEST_ASSERT_TRUE(newState); // THEN true as group always return true
}

void whenGroupUpdatingState()
{
    groupUpdateState(group);

    TEST_ASSERT_TRUE(child00State.updated); // THEN all children states updated
    TEST_ASSERT_TRUE(child01State.updated); // THEN all children states updated
    TEST_ASSERT_TRUE(child02State.updated); // THEN all children states updated
}

void whenTick()
{
    const unsigned long TICK = 4583;
    groupOnTick(group, TICK);

    TEST_ASSERT_EQUAL_UINT64(TICK, child00State.tick);
    TEST_ASSERT_EQUAL_UINT64(TICK, child01State.tick);
    TEST_ASSERT_EQUAL_UINT64(TICK, child02State.tick);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenMount);
    RUN_TEST(whenContains);
    RUN_TEST(whenGroupContainsGroupWithChild);
    RUN_TEST(whenNotContains);
    RUN_TEST(whenRender);
    RUN_TEST(whenRenderNotChangedChildren);
    RUN_TEST(whenRenderNotChangedChildrenInForcedMode);    
    RUN_TEST(whenNewState);
    RUN_TEST(whenAllChildrenNewStateReturnedFalse);
    RUN_TEST(whenNotAllChildrenNewStateReturnedFalse);
    RUN_TEST(whenGroupUpdatingState);
    RUN_TEST(whenTick);
    UNITY_END();
}