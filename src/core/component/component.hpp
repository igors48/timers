/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief UI Component 
*/
#pragma once

#include "../watch/tft.hpp"

typedef struct Component_ Component;

typedef enum { // todo prefixes to avoid collisions #123
    NONE,
    MOVE_UP,
    MOVE_LEFT,
    MOVE_DOWN,
    MOVE_RIGHT,
} Gesture;

// always wrote here in comment color in RGB scheme
const unsigned int COLOR_BLACK = 0x0000; 
const unsigned int COLOR_INFORMATION = 0xFDA0;
const unsigned int COLOR_INTERACTION = 0x03E0;
const unsigned int COLOR_ATTENTION = 0xF800;
const unsigned int COLOR_BUTTON_TEXT = 0xFFFF;
const unsigned int COLOR_BUTTON_BACK_RELEASED = 0x8C50;
const unsigned int COLOR_BUTTON_BACK_PRESSED = 0xD6B9;
const unsigned int COLOR_BUTTON_BACK_DISABLED = 0xF800;

/**
 * @brief definition of a Component
 * 
 */
struct Component_
{
    signed short x; // TODO check why signed ? what is the reason? #94
    signed short y;
    signed short w;
    signed short h;
    Component* (*contains)(Component *component, signed short x, signed short y);
    void (*mount)(Component *component, signed short x, signed short y);
    void (*onTouch)(Component *component, signed short x, signed short y, unsigned long tickCount); // todo consider interface EventListener #121
    void (*onMove)(Component *component, signed short x, signed short y, unsigned long tickCount);  
    void (*onRelease)(Component *component, signed short x, signed short y, unsigned long tickCount);  
    void (*onGesture)(Component *component, Gesture gesture); // todo gesture and button events are tile level events. consider remove handlers from component #122
    void (*onButton)(Component *component);
    void (*render)(Component *component, bool forced, TftApi *tftApi);  // todo consider interface Renderable #121
    bool (*isStateModified)(Component *component);
    void (*updateState)(Component *component);
    void *state;    
};

void componentNoopHandler(Component *component, signed short x, signed short y, unsigned long tickCount);

void componentGestureNoopHandler(Component *component, Gesture gesture);

void componentButtonNoopHandler(Component *component);

void componentRenderNoop(Component *component, bool forced, TftApi *tftApi);

bool componentIsStateModifiedNoop(Component *component);

void componentUpdateStateNoop(Component *component);

Component* componentContains(Component *component, signed short x, signed short y);

void componentMount(Component *component, signed short x, signed short y);

Component createComponent(signed short x, signed short y, signed short w, signed short h, void *state);