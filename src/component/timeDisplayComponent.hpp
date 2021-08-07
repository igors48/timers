#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short color;
    unsigned short _color;
    unsigned char _hour;
    unsigned char _minute;
    unsigned char _second;
} TimeDisplayComponentState;

Component createTimeDisplayComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, TimeDisplayComponentState *state);