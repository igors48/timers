#pragma once

#include "component.hpp"

typedef void (*Handler)();

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
    Handler handler;
    unsigned int delayTick;
    unsigned int repeatTick;
    EventHandlingState eventHandlingState;
    EventHandlingState _eventHandlingState;
    unsigned int firstTouchTick;
    unsigned int lastRepeatTick;
} ButtonComponentState;

ButtonComponentState createButtonState(char *title, EventGenerate eventGenerate, Handler handler);

Component createButtonComponent(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state);