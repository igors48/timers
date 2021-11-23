#include <unity.h>

#include "component/component.cpp"
#include "component/screenComponent.cpp"

typedef struct
{
    bool rendered;
} TileState;

const unsigned char TILES_COUNT = 2;

TileState firstTileState;
Component firstTile;
TileState secondTileState;
Component secondTile;
ScreenState screenState;
void* tiles[TILES_COUNT];
Component screenComponent;

void tileRenderStub(Component *component, bool forced, WatchState *watchState, TftApi *tftApi)
{
    TileState *state = (TileState *)component->state;
    state->rendered = true;
}

void setUp(void)
{
    firstTileState = {
        .rendered = false,
    };
    firstTile = {};
    firstTile.state = &firstTileState;
    firstTile.render = tileRenderStub;
    
    secondTileState = {
        .rendered = false,
    };
    secondTile = {};
    secondTile.state = &secondTileState;
    secondTile.render = tileRenderStub;

    tiles[0] = &firstTile;
    tiles[1] = &secondTile;

    screenState = createScreenState(TILES_COUNT, tiles);
    screenComponent = createScreenComponent(&screenState);
}

// void whenRender()
// {
//     screenComponentRender(&screenComponent, false, NULL, NULL);
// }

void whenNewStateAndActiveTileTheSame()
{
    screenState._activeTile = 0;

    bool newState = screenComponentNewState(&screenComponent, NULL);

    TEST_ASSERT_EQUAL_UINT8(0, newState);
}

void whenNewStateAndActiveTileChanged()
{
    screenState._activeTile = 42;

    bool newState = screenComponentNewState(&screenComponent, NULL);

    TEST_ASSERT_EQUAL_UINT8(1, newState);
}

int main()
{
    UNITY_BEGIN();
    // RUN_TEST(whenRender);
    RUN_TEST(whenNewStateAndActiveTileTheSame);
    RUN_TEST(whenNewStateAndActiveTileChanged);
    UNITY_END();
}