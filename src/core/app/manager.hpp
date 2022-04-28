#pragma once

#include "core/app/tiler.hpp"

typedef void (*ActivateApp)(unsigned char index);

typedef struct 
{
    ActivateApp activateApp;
} Manager;

Manager createManager(unsigned char appsCount, void **apps, Tiler *tilerRef);