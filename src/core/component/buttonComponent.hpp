#pragma once

#include "component.hpp"

typedef void (*Handler)(void *context);

typedef enum {
    BM_INIT,
    BM_ENABLED,
    BM_DISABLED,
} ButtonMode;

typedef enum {
    EG_ONCE,
    EG_REPEAT,
} EventGenerate;

typedef enum {
    EHS_INIT,
    EHS_IDLE,
    EHS_PRESS,
    EHS_LEAVE,
    EHS_REPEAT,
} EventHandlingState;

typedef struct
{
    char *title;
    EventGenerate eventGenerate;
    void *context;
    Handler handler;
    unsigned int delayTick;
    unsigned int repeatTick;
    EventHandlingState eventHandlingState;
    EventHandlingState _eventHandlingState;
    ButtonMode mode;
    ButtonMode _mode;
    unsigned int firstTouchTick;
    unsigned int lastRepeatTick;
} ButtonComponentState;

void buttonOnTouch(Component *component, signed short x, signed short y, unsigned int tickCount);
void buttonOnMove(Component *component, signed short x, signed short y, unsigned int tickCount);
void buttonOnRelease(Component *component, signed short x, signed short y, unsigned int tickCount);
void buttonRender(Component *component, bool forced, TftApi *tftApi);
bool buttonIsStateModified(Component *component);
void buttonUpdateState(Component *component);