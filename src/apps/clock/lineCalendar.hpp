#pragma once

#include "core/component/component.hpp"

typedef struct LineCalendarContext_ LineCalendarContext;

typedef void (*ProvideContext)(LineCalendarContext *state); // todo generic state provider function definition

struct LineCalendarContext_
{
    unsigned int dayOfWeek;
    unsigned int _dayOfWeek;
    ProvideContext provideContext;
};

LineCalendarContext createLineCalendarContext(ProvideContext provideContext);

Component createLineCalendarComponent(signed short x, signed short y, LineCalendarContext *context);
