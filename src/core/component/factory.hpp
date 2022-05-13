#pragma once

#include "core/system/system.hpp"

#include "buttonComponent.hpp"

typedef struct 
{
    ButtonComponentState* (*createButtonStateRef)(char *title, EventGenerate eventGenerate, Handler handler);
    Component* (*createButtonComponentRef)(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state);
} Factory;

Factory createFactory(SystemApi *systemApiRef);