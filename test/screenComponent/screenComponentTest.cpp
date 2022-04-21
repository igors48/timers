#include <unity.h>

#include "core/component/component.cpp"
#include "core/component/screenComponent.cpp"

typedef struct
{
    bool rendered;
    bool forcedRender;
    bool mounted;
    bool containsCalled;
    Gesture gesture;
} TileState;

const unsigned char TILES_COUNT = 2;

TileState firstTileState;
Component firstTile;
TileState secondTileState;
Component secondTile;
ScreenState screenState;
void* tiles[TILES_COUNT];
Component screenComponent;
TftApi tftApi;
bool fillRectCalled;

void tileRenderStub(Component *component, bool forced, WatchState *watchState, TftApi *tftApi)
{
    TileState *state = (TileState *)component->state;
    state->rendered = true;
    state->forcedRender = forced;
}

void tileMountStub(Component *component, signed short x, signed short y)
{
    TileState *state = (TileState *)component->state;
    state->mounted = true;    
}

Component* tileContainsStub(Component *component, signed short x, signed short y)
{
    TileState *state = (TileState *)component->state;
    state->containsCalled = true;

    return component;
}

void tileGestureStub(Component *component, Gesture gesture)
{
    TileState *state = (TileState *)component->state;
    state->gesture = gesture;
}

void fillRectStub(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    fillRectCalled = true;
}

void setUp(void)
{
    fillRectCalled = false;
    tftApi = {};
    tftApi.fillRect = fillRectStub;

    firstTileState = {
        .rendered = false,
        .forcedRender = false,
        .mounted = false,
        .containsCalled = false,
        .gesture = NONE,
    };
    firstTile = {};
    firstTile.state = &firstTileState;
    firstTile.render = tileRenderStub;
    firstTile.mount = tileMountStub;
    firstTile.contains = tileContainsStub;
    firstTile.onGesture = tileGestureStub;
    
    secondTileState = {
        .rendered = false,
        .forcedRender = false,
        .mounted = false,
        .containsCalled = false,
        .gesture = NONE,
    };
    secondTile = {};
    secondTile.state = &secondTileState;
    secondTile.render = tileRenderStub;
    secondTile.mount = tileMountStub;
    secondTile.contains = tileContainsStub;
    secondTile.onGesture = tileGestureStub;

    tiles[0] = &firstTile;
    tiles[1] = &secondTile;

    screenState = createScreenState(TILES_COUNT, tiles);
    screenComponent = createScreenComponent(&screenState);
}

void whenRenderAndActiveTileTheSame()
{
    screenState._activeTile = screenState.activeTile;

    screenComponentRender(&screenComponent, false, NULL, &tftApi);

    TEST_ASSERT_FALSE(fillRectCalled); // THEN screen not cleared
    TEST_ASSERT_TRUE(firstTileState.rendered); // THEN active tile rendered
    TEST_ASSERT_FALSE(firstTileState.forcedRender); // THEN active tile rendered in not forced mode 
    TEST_ASSERT_FALSE(secondTileState.rendered); // THEN inactive tile not rendered
    TEST_ASSERT_FALSE(secondTileState.forcedRender); // THEN inactive tile not rendered 
}

void whenRenderAndActiveTileChanged()
{
    screenState.activeTile = 1;

    screenComponentRender(&screenComponent, false, NULL, &tftApi);

    TEST_ASSERT_TRUE(fillRectCalled); // THEN screen cleared
    TEST_ASSERT_FALSE(firstTileState.rendered); // THEN inactive tile not rendered
    TEST_ASSERT_FALSE(firstTileState.forcedRender); // THEN inactive tile not rendered 
    TEST_ASSERT_TRUE(secondTileState.rendered); // THEN active tile rendered
    TEST_ASSERT_TRUE(secondTileState.forcedRender); // THEN active tile rendered in forced mode 
}

void whenNewStateAndActiveTileTheSame()
{
    screenState._activeTile = 0;

    bool newState = screenComponentNewState(&screenComponent, NULL);

    TEST_ASSERT_EQUAL_UINT8(0, newState); // THEN this is old state
}

void whenNewStateAndActiveTileChanged()
{
    screenState._activeTile = 42;

    bool newState = screenComponentNewState(&screenComponent, NULL);

    TEST_ASSERT_EQUAL_UINT8(1, newState); // THEN this is new state
}

void whenMount()
{
    screenComponentMount(&screenComponent, 0, 0);

    TEST_ASSERT_EQUAL_UINT8(1, firstTileState.mounted); // THEN all tiles mounted
    TEST_ASSERT_EQUAL_UINT8(1, secondTileState.mounted); // THEN all tiles mounted 
}

void whenContains()
{
    screenState.activeTile = 1;

    screenComponentContains(&screenComponent, 42, 42);

    TEST_ASSERT_EQUAL_UINT8(0, firstTileState.containsCalled); // THEN not active tile not touched
    TEST_ASSERT_EQUAL_UINT8(1, secondTileState.containsCalled); // THEN active tile contains called
}

void whenGesture()
{
    screenState.activeTile = 1;

    screenComponentGestureEventHandler(&screenComponent, MOVE_DOWN);

    TEST_ASSERT_TRUE(firstTileState.gesture == NONE); // THEN not active tile not touched
    TEST_ASSERT_TRUE(secondTileState.gesture == MOVE_DOWN); // THEN active tile contains called
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenRenderAndActiveTileTheSame);
    RUN_TEST(whenRenderAndActiveTileChanged);
    RUN_TEST(whenNewStateAndActiveTileTheSame);
    RUN_TEST(whenNewStateAndActiveTileChanged);
    RUN_TEST(whenMount);
    RUN_TEST(whenContains);
    RUN_TEST(whenGesture);
    UNITY_END();
}