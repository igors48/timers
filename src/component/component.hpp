#pragma once

#include "../task/watchState.hpp"
#include "../watch/tft.hpp"

typedef struct Component_ Component;

typedef void (*Render)(Component component, WatchState watchState, TftApi *tftApi);
typedef void (*OnTouch)(Component component);
typedef bool (*NewState)(Component component, WatchState watchState);

struct Component_
{
    unsigned char x;
    unsigned char y;
    unsigned char w;
    unsigned char h;
    void *state;    
    Render render;  
    OnTouch onTouch;  
    NewState newState;
};

void componentOnTouch(Component component);

bool componentNewState(Component component, WatchState watchState);