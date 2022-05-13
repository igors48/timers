#pragma once

#include "core/system/system.hpp"

#include "buttonComponent.hpp"

typedef struct 
{
    ButtonComponentState* (*createButtonStateRef)(char *title, EventGenerate eventGenerate, Handler handler);
} Factory;

Factory createFactory(SystemApi *systemApiRef);