#pragma once

#include "component.hpp"

typedef struct
{
    unsigned short color;

    unsigned short _color;
    unsigned char _hour;
    unsigned char _minute;
} HourMinuteComponentState;

Component createHourMinuteComponent(signed short x, signed short y, signed short w, signed short h, HourMinuteComponentState *state);

void hourMinuteComponentColorChange(HourMinuteComponentState *state);
