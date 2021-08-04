#pragma once

#include "../task/watchState.hpp"
#include "../watch/tft.hpp"

typedef void (*Render)(void *componentState, WatchState watchState, TftApi *tftApi);

typedef struct 
{
    void *state;
    Render render;    
} Component;
