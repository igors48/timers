#pragma once

#include "component.hpp"

typedef void (*Handler)();

typedef struct
{
    char *title;
    bool pressed;
    bool _pressed;
    Handler handler;
} ButtonComponentState;

ButtonComponentState createButtonState(char *title, Handler handler);

Component createButtonComponent(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state);