#pragma once

#include "../watch/tft.hpp"

typedef struct Component_ Component;

typedef enum {
    NONE,
    MOVE_UP,
    MOVE_LEFT,
    MOVE_DOWN,
    MOVE_RIGHT,
} Gesture;

typedef void (*Render)(Component *component, bool forced, TftApi *tftApi);
typedef bool (*NewState)(Component *component);
typedef void (*TouchEventHandler)(Component *component, signed short x, signed short y);
typedef void (*GestureEventHandler)(Component *component, Gesture gesture);
typedef Component* (*Contains)(Component *component, signed short x, signed short y);
typedef void (*Mount)(Component *component, signed short x, signed short y);

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
    Contains contains;
    Mount mount;
    TouchEventHandler onTouch; // todo consider interface EventListener 
    TouchEventHandler onMove;  
    TouchEventHandler onRelease;  
    GestureEventHandler onGesture;
    Render render;  // todo consider interface Renderable
    NewState newState; // todo consider better name
    void *state;    
};

void componentNoopHandler(Component *component, signed short x, signed short y);

void componentGestureNoopHandler(Component *component, Gesture gesture);

bool componentNewState(Component *component);

Component* componentContains(Component *component, signed short x, signed short y);

void componentMount(Component *component, signed short x, signed short y);