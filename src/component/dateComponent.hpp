#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short _year;
    unsigned char _month;
    unsigned char _day;
} DateComponentState;

Component createDateComponent(signed short x, signed short y, signed short w, signed short h, DateComponentState *state);
