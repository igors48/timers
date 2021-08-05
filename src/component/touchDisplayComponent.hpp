#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char x;
    unsigned char y;
} TouchDisplayComponentState;

Component createTouchDisplayComponent(TouchDisplayComponentState *state);