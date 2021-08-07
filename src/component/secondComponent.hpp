#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short color;

    unsigned short _color;
    unsigned char _second;
} SecondComponentState;

Component createSecondComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, SecondComponentState *state);