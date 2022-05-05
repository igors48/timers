#include <stddef.h>

#include "component.hpp"
#include "buttonComponent.hpp"

static void render(Component *component, bool forced, TftApi *tftApi)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    unsigned int rectColor = COLOR_BLACK;
    if (state->eventHandlingState == EHS_PRESS)
    {
        rectColor = COLOR_INTERACTION;
    }
    (tftApi->drawRect)(component->x, component->y, component->w, component->h, rectColor);
    (tftApi->setCursor)(component->x + 4, component->y + 4);
    (tftApi->setTextSize)(2);
    (tftApi->setTextFont)(1);
    (tftApi->setTextColor)(COLOR_INTERACTION, COLOR_BLACK);
    (tftApi->print)(state->title);
}

static bool newState(Component *component)
{
    ButtonComponentState *state = (ButtonComponentState *)component->state;

    bool changed = false;

    if (state->eventHandlingState != state->_eventHandlingState)
    {
        changed = true;
        state->_eventHandlingState = state->eventHandlingState;
    }

    return changed;
}

static void onTouch(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    state->eventHandlingState = EHS_PRESS;
    state->firstTouchTick = tickCount;
}

static void onMove(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    bool itsMe = (component->contains)(component, x, y) != NULL;
    if (!itsMe)
    {
        state->eventHandlingState = EHS_LEAVE;
        return;     
    }
    if (state->eventGenerate == EG_ONCE)
    {

    }
    else
    {

    } 
}

static void onRelease(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    bool itsMe = (component->contains)(component, x, y) != NULL;
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    bool noRepeat = state->eventHandlingState != EHS_REPEAT;
    if (itsMe && noRepeat)
    {
        (state->handler)();
    }
    state->eventHandlingState = EHS_IDLE;
}

ButtonComponentState createButtonState(char *title, Handler handler)
{
    return {
        .title = title,
        .eventGenerate = EG_ONCE,
        .handler = handler,
        .delayTick = 1000, // todo pass as a parameter. depends on portTICK_PERIOD_MS
        .repeatTick = 500, // todo pass as a parameter. depends on portTICK_PERIOD_MS
        .eventHandlingState = EHS_IDLE,
        ._eventHandlingState = EHS_INIT,
        .firstTouchTick = 0,
        .lastRepeatTick = 0,
    };
}

Component createButtonComponent(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .contains = componentContains,
        .mount = componentMount,
        .onTouch = onTouch,
        .onMove = onMove,
        .onRelease = onRelease,
        .onGesture = componentGestureNoopHandler,
        .render = render,
        .newState = newState,
        .state = state,
    };
}
