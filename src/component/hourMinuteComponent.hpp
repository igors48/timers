#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short color;

    unsigned short _color;
    unsigned char _hour;
    unsigned char _minute;
} HourMinuteComponentState;

Component createHourMinuteComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, HourMinuteComponentState *state);

void hourMinuteComponentColorChange(HourMinuteComponentState *state);
