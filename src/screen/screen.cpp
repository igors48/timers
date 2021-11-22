#include "component/screenComponent.hpp"

#include "screen.hpp"
#include "clockTile.hpp"
#include "stepCounterTile.hpp"

static Component clockTile;
static Component stepCounterTile;

static const unsigned char TILES_COUNT = 2;
static void* tiles[TILES_COUNT];
static ScreenState screenState;
static Component screen;

static void setActiveTile(unsigned short activeTile)
{
    screenState.activeTile = activeTile;
}

Component createScreen()
{
    clockTile = createClockTile(setActiveTile);
    stepCounterTile = createStepCounterTile(setActiveTile);

    tiles[0] = &clockTile;
    tiles[1] = &stepCounterTile;

    screenState = {
        .tilesCount = TILES_COUNT,
        .tiles = tiles,
        .activeTile = 0,
        ._activeTile = 65535,
    };

    screen = createScreenComponent(&screenState);
    screen.mount(&screen, 0, 0);

    return screen;
}