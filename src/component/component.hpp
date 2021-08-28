#pragma once

#include "../task/watchState.hpp"
#include "../watch/tft.hpp"

typedef struct Component_ Component;

typedef void (*Render)(Component *component, WatchState *watchState, TftApi *tftApi);
typedef bool (*NewState)(Component *component, WatchState *watchState);
typedef void (*TouchEventHandler)(Component *component, signed short x, signed short y);

const unsigned int COLOR_BLACK = 0x0000;
const unsigned int COLOR_INFORMATION = 0xFDA0;
const unsigned int COLOR_INTERACTION = 0x03E0;
const unsigned int COLOR_ATTENTION = 0xF800;

struct Component_
{
    signed short x;
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

bool componentNewState(Component *component, WatchState *watchState);