#pragma once

#include "component.hpp"

typedef struct
{
    unsigned char x;
    unsigned char y;
} TimeDisplayComponentState;

Component createTimeDisplayComponent(TimeDisplayComponentState *state);