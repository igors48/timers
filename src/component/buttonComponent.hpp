#pragma once

#include "component.hpp"

typedef void (*Handler)();

typedef struct
{
    bool pressed;
    bool _pressed;
    Handler handler;
} ButtonComponentState;

Component createButtonComponent(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state);