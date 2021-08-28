#pragma once

#include "component.hpp"

typedef void (*Handler)();

typedef struct
{
    bool pressed;
    Handler handler;
} ButtonComponentState;

Component createButtonComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, ButtonComponentState *state);