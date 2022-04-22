#include "core/component/screenComponent.hpp"

#include "screen.hpp"

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

Component createScreen(SoundApi *soundApi)
{
    tiles[0] = &clockTile;
    tiles[1] = &stepCounterTile;

    screenState = createScreenState(TILES_COUNT, tiles);
    screen = createScreenComponent(&screenState);
    screen.mount(&screen, 0, 0);

    return screen;
}