#pragma once

#include "../task/watchState.hpp"
#include "../watch/tft.hpp"

typedef struct Component_ Component;

typedef void (*Render)(Component component, WatchState watchState, TftApi *tftApi);

struct Component_
{
    unsigned char x;
    unsigned char y;
    unsigned char w;
    unsigned char h;
    Render render;    
};

