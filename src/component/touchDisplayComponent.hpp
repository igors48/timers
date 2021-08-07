#pragma once

#include "component.hpp"

typedef struct 
{
    signed short _touchX;
    signed short _touchY;
} TouchDisplayComponentState;

Component createTouchDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, TouchDisplayComponentState *state);