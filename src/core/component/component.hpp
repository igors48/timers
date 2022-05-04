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
    Component* (*contains)(Component *component, signed short x, signed short y);
    void (*mount)(Component *component, signed short x, signed short y);
    void (*onTouch)(Component *component, signed short x, signed short y, unsigned int tickCount); // todo consider interface EventListener 
    void (*onMove)(Component *component, signed short x, signed short y, unsigned int tickCount);  
    void (*onRelease)(Component *component, signed short x, signed short y, unsigned int tickCount);  
    void (*onGesture)(Component *component, Gesture gesture);
    void (*render)(Component *component, bool forced, TftApi *tftApi);  // todo consider interface Renderable
    bool (*newState)(Component *component); // todo consider better name
    void *state;    
};

void componentNoopHandler(Component *component, signed short x, signed short y, unsigned int tickCount);

void componentGestureNoopHandler(Component *component, Gesture gesture);

bool componentNewState(Component *component);

Component* componentContains(Component *component, signed short x, signed short y);

void componentMount(Component *component, signed short x, signed short y);
