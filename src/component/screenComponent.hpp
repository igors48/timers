#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short tilesCount;
    void **tiles;
    unsigned short activeTile;
    unsigned short _activeTile;
} ScreenState;

Component createScreenComponent(ScreenState *state);
