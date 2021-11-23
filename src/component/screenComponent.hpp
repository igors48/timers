#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short tilesCount;
    void **tiles;
    unsigned short activeTile;
    unsigned short _activeTile;
} ScreenState;

ScreenState createScreenState(unsigned short tilesCount, void **tiles);

Component createScreenComponent(ScreenState *state);
