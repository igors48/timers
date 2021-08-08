#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short _year;
    unsigned char _month;
    unsigned char _day;
} DateComponentState;

Component createDateComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, DateComponentState *state);