#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short color;

    unsigned short _color;
    unsigned char _second;
} SecondComponentState;

Component createSecondComponent(signed short x, signed short y, signed short w, signed short h, SecondComponentState *state);

void secondComponentColorChange(SecondComponentState *state);