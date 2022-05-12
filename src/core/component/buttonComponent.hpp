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

ButtonComponentState createButtonState(char *title, EventGenerate eventGenerate, Handler handler);

ButtonComponentState* createButtonStateRef(char *title, EventGenerate eventGenerate, Handler handler);

Component createButtonComponent(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state);