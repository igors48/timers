#pragma once

#include "core/app/tiler.hpp"

typedef void (*ActivateApp)(unsigned char index);
typedef void (*ActivatePrevApp)();
typedef void (*ActivateNextApp)();

typedef struct 
{
    ActivateApp activateApp;
    ActivatePrevApp activatePrevApp;
    ActivateNextApp activateNextApp;    
} Manager;

Manager createManager(unsigned char appsCount, void **apps, Tiler *tiler);