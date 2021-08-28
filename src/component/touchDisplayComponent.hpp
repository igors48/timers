#pragma once

#include "component.hpp"

typedef struct 
{
    signed short _touchX;
    signed short _touchY;
} TouchDisplayComponentState;

Component createTouchDisplayComponent(signed short x, signed short y, signed short w, signed short h, TouchDisplayComponentState *state);