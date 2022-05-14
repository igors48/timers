#pragma once

#include "core/system/system.hpp"

#include "group.hpp"
#include "textComponent.hpp"
#include "buttonComponent.hpp"
#include "stepperComponent.hpp"

typedef struct 
{
    GroupState* (*createGroupStateRef)(unsigned char childrenCount, void **children);
    Component* (*createGroupComponentRef)(signed short x, signed short y, GroupState *state);

    TextState* (*createTextStateRef)(unsigned char font, unsigned char size, unsigned int fontColor, Provide provide);
    Component* (*createTextComponentRef)(signed short x, signed short y, signed short w, signed short h, TextState *state);

    ButtonComponentState* (*createButtonStateRef)(char *title, EventGenerate eventGenerate, Handler handler);
    Component* (*createButtonComponentRef)(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state);

    StepperComponentState* (*createStepperComponentStateRef)(signed short min, signed short max, signed short value, OnStepperChange onChange);
    Component* (*createStepperComponentRef)(signed short x, signed short y, StepperComponentState *state);
} Factory;

Factory createFactory(SystemApi *systemApiRef);