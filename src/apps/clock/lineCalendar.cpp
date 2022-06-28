
#include "lineCalendar.hpp"

typedef struct
{
    unsigned short offset;
    char *text;
} WeekDayIcon;

static const unsigned char WEEK_DAY_COUNT = 7;
static const unsigned short X_DIFF = 34;
static const unsigned short X_SHIFT = 5;
static const WeekDayIcon ICONS[] = {
    {
        .offset = X_SHIFT + X_DIFF * 6,
        .text = (char *)"SU",
    },
    {
        .offset = X_SHIFT,
        .text = (char *)"MO",
    },
    {
        .offset = X_SHIFT + X_DIFF + 2,
        .text = (char *)"TU",
    },
    {
        .offset = X_SHIFT + X_DIFF * 2,
        .text = (char *)"WE",
    },
    {
        .offset = X_SHIFT + X_DIFF * 3 + 2,
        .text = (char *)"TH",
    },
    {
        .offset = X_SHIFT + X_DIFF * 4 + 2,
        .text = (char *)"FR",
    },
    {
        .offset = X_SHIFT + X_DIFF * 5,
        .text = (char *)"SA",
    },
};

static void render(Component *component, bool forced, TftApi *tftApi)
{
    LineCalendarContext *context = (LineCalendarContext *)component->state;
    (tftApi->setTextSize)(0);
    (tftApi->setTextFont)(MEDIUM_FONT);
    (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    for (int i = 0; i < WEEK_DAY_COUNT; i++)
    {
        if (context->dayOfWeek == i)
        {
            (tftApi->setTextColor)(COLOR_INFORMATION, COLOR_BLACK);
        }
        else
        {
            (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
        }
        const WeekDayIcon icon = ICONS[i];
        (tftApi->drawString)(icon.text, component->x + icon.offset, component->y);
    }
}

static bool isStateModified(Component *component) // todo create dummies for isStateModified & updateState & render
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
