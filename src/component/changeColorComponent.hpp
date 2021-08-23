#pragma once

#include "component.hpp"

typedef void (*Handler)();

typedef struct
{
    Handler handler;
} ChangeColorComponentState;

Component createChangeColorComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, ChangeColorComponentState *state);