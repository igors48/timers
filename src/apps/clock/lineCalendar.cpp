
#include "lineCalendar.hpp"

static void render(Component *component, bool forced, TftApi *tftApi)
{
    LineCalendarContext *context = (LineCalendarContext *)component->state;
    (tftApi->setTextSize)(0);
    (tftApi->setTextFont)(MEDIUM_FONT);
    (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    const unsigned short xDiff = 34;
    const unsigned short xShift = 5;
    if (context->dayOfWeek == 1)
    {
        (tftApi->setTextColor)(COLOR_INFORMATION, COLOR_BLACK);
    }
    else
    {
        (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    }
    (tftApi->drawString)("MO", component->x + xShift, component->y);
    if (context->dayOfWeek == 2)
    {
        (tftApi->setTextColor)(COLOR_INFORMATION, COLOR_BLACK);
    }
    else
    {
        (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    }
    (tftApi->drawString)("TU", component->x + xShift + xDiff + 2, component->y);
    if (context->dayOfWeek == 3)
    {
        (tftApi->setTextColor)(COLOR_INFORMATION, COLOR_BLACK);
    }
    else
    {
        (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    }
    (tftApi->drawString)("WE", component->x + xShift + xDiff * 2, component->y);
    if (context->dayOfWeek == 4)
    {
        (tftApi->setTextColor)(COLOR_INFORMATION, COLOR_BLACK);
    }
    else
    {
        (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    }
    (tftApi->drawString)("TH", component->x + xShift + xDiff * 3 + 2, component->y);
    if (context->dayOfWeek == 5)
    {
        (tftApi->setTextColor)(COLOR_INFORMATION, COLOR_BLACK);
    }
    else
    {
        (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    }
    (tftApi->drawString)("FR", component->x + xShift + xDiff * 4 + 2, component->y);
    if (context->dayOfWeek == 6)
    {
        (tftApi->setTextColor)(COLOR_INFORMATION, COLOR_BLACK);
    }
    else
    {
        (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    }
    (tftApi->drawString)("SA", component->x + xShift + xDiff * 5, component->y);
    if (context->dayOfWeek == 0)
    {
        (tftApi->setTextColor)(COLOR_INFORMATION, COLOR_BLACK);
    }
    else
    {
        (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    }
    (tftApi->drawString)("SU", component->x + xShift + xDiff * 6, component->y);
}

static bool isStateModified(Component *component) // todo create dummies for isStateModified & updateState
{
    LineCalendarContext *context = (LineCalendarContext *)(component->state);
    (context->provideContext)(context);
    return context->_dayOfWeek != context->dayOfWeek;
}

static void updateState(Component *component)
{
    LineCalendarContext *context = (LineCalendarContext *)(component->state);
    context->_dayOfWeek = context->dayOfWeek;
}

LineCalendarContext createLineCalendarContext(ProvideContext provideContext)
{
    return {
        .dayOfWeek = 100,
        ._dayOfWeek = 101,
        .provideContext = provideContext,
    };
}

Component createLineCalendarComponent(signed short x, signed short y, LineCalendarContext *context)
{
    return {
        // todo create default factory for Component
        .x = x,
        .y = y,
        .w = 240,
        .h = 50,
        .contains = componentContains,
        .mount = componentMount,
        .onTouch = componentNoopHandler,
        .onMove = componentNoopHandler,
        .onRelease = componentNoopHandler,
        .onGesture = componentGestureNoopHandler,
        .onButton = componentButtonNoopHandler,
        .render = render,
        .isStateModified = isStateModified,
        .updateState = updateState,
        .state = context,
    };
}
