#pragma once

#include "../task/watchState.hpp"
#include "../watch/tft.hpp"

typedef void (*Render)(WatchState watchState, TftApi *tftApi);

typedef struct 
{
    void *props;
    Render render;    
} Component;
