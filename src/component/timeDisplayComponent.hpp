#pragma once

#include "component.hpp"

typedef struct 
{
    unsigned short color;
} TimeDisplayComponentState;

Component createTimeDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, TimeDisplayComponentState *state);