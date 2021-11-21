#include "component/screenComponent.hpp"

#include "screen.hpp"
#include "clockTile.hpp"
#include "stepCounterTile.hpp"

Component clockTile;
Component stepCounterTile;

const unsigned char TILES_COUNT = 2;
void* tiles[TILES_COUNT];
ScreenState screenState;

void setActiveTile(unsigned short activeTile)
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

    return createScreenComponent(&screenState);
}