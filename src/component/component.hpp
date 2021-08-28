#pragma once

#include "../task/watchState.hpp"
#include "../watch/tft.hpp"

typedef struct Component_ Component;

// todo use pointer to a Component instead copy
// todo use pointer to a WatchState instead copy
typedef void (*Render)(Component component, WatchState watchState, TftApi *tftApi);
typedef bool (*NewState)(Component component, WatchState watchState);
typedef void (*TouchEventHandler)(Component *component, signed short x, signed short y);

struct Component_
{
    signed short x; // todo consider short
    signed short y;
    signed short w;
    signed short h;
    TouchEventHandler onTouch; // todo consider interface EventListener 
    TouchEventHandler onMove;  
    TouchEventHandler onRelease;  
    Render render;  // todo consider interface Renderable
    NewState newState; // todo consider better name
    void *state;    
};

void componentNoopHandler(Component *component, signed short x, signed short y);

bool componentNewState(Component component, WatchState watchState);