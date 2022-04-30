#pragma once

#include "core/app/tiler.hpp"

typedef struct 
{
    void (*activateApp)(unsigned char index);
} Manager;

Manager createManager(unsigned char appsCount, void **apps, Tiler *tilerRef);