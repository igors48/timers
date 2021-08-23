#pragma once

#include "component.hpp"

typedef struct
{

} ChangeColorComponentState;

Component createChangeColorComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, ChangeColorComponentState *state);