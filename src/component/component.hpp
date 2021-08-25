#pragma once

#include "../task/watchState.hpp"
#include "../watch/tft.hpp"

typedef struct Component_ Component;

// todo use pointer to a Component instead copy
// todo use pointer to a WatchState instead copy
typedef void (*Render)(Component component, WatchState watchState, TftApi *tftApi);
typedef bool (*NewState)(Component component, WatchState watchState);
typedef void (*TouchEventHandler)(Component component);

struct Component_
{
    unsigned char x;
    unsigned char y;
    unsigned char w;
    unsigned char h;
    TouchEventHandler onTouch;  
    TouchEventHandler onRelease;  
    TouchEventHandler onSkip;  
    Render render;  
    NewState newState; // todo consider better name
    void *state;    
};

void componentNoopHandler(Component component);

bool componentNewState(Component component, WatchState watchState);