#include <stddef.h>

#include "component.hpp"
#include "buttonComponent.hpp"

static void firstRepeat(ButtonComponentState *state, unsigned long tickCount)
{
    const unsigned long pressedTick = tickCount - state->firstTouchTick;
    if (pressedTick > state->delayTick)
    {
        state->eventHandlingState = EHS_REPEAT;
        state->lastRepeatTick = tickCount;
        (state->handler)(state->context);
    }
}

static void notFirstRepeat(ButtonComponentState *state, unsigned long tickCount)
{
    const unsigned long fromLastRepeatTick = tickCount - state->lastRepeatTick;
    if (fromLastRepeatTick > state->repeatTick)
    {
        state->lastRepeatTick = tickCount;
        (state->handler)(state->context);
    }
}

static void repeat(ButtonComponentState *state, unsigned long tickCount)
{
    if (state->eventHandlingState == EHS_PRESS)
    {
        firstRepeat(state, tickCount);
    }
    else
    {
        notFirstRepeat(state, tickCount);
    }
}

static void press(ButtonComponentState *state, unsigned long tickCount)
{
    if (state->eventHandlingState == EHS_LEAVE)
    {
        state->eventHandlingState = EHS_PRESS;
    }
}

static void onEnter(ButtonComponentState *state, unsigned long tickCount)
{
    if (state->eventGenerate == EG_REPEAT)
    {
        repeat(state, tickCount);
    }
    else
    {
        press(state, tickCount);
    }
}

static void onLeave(ButtonComponentState *state)
{
    if (state->eventHandlingState == EHS_PRESS || state->eventHandlingState == EHS_REPEAT)
    {
        state->eventHandlingState = EHS_LEAVE;
    }
}

void buttonOnTouch(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    if (state->mode == BM_DISABLED)
    {
        state->eventHandlingState = EHS_IDLE;
        return;
    }
    state->eventHandlingState = EHS_PRESS;
    state->firstTouchTick = tickCount;
}

void buttonOnMove(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    if (state->mode == BM_DISABLED)
    {
        state->eventHandlingState = EHS_IDLE;
        return;
    }
    const bool itsMe = (component->contains)(component, x, y) != NULL;
    if (itsMe)
    {
        onEnter(state, tickCount);
    }
    else
    {
        onLeave(state);
    }
}

void buttonOnRelease(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    if (state->mode == BM_DISABLED)
    {
        state->eventHandlingState = EHS_IDLE;
        return;
    }
    const bool itsMe = (component->contains)(component, x, y) != NULL;
    const bool noRepeat = state->eventHandlingState != EHS_REPEAT;
    if (itsMe && noRepeat)
    {
        (state->handler)(state->context);
    }
    state->eventHandlingState = EHS_IDLE;
}

void buttonRender(Component *component, bool forced, TftApi *tftApi)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    unsigned int rectColor = COLOR_BUTTON_BACK_RELEASED;
    if (state->eventHandlingState == EHS_PRESS || state->eventHandlingState == EHS_REPEAT)
    {
        rectColor = COLOR_BUTTON_BACK_PRESSED;
    }
    if (state->mode == BM_DISABLED)
    {
        rectColor = COLOR_BUTTON_BACK_DISABLED;
    }
    (tftApi->fillRoundRect)(component->x, component->y, component->w, component->h, 4, rectColor); // todo make radius a parameter or const
    (tftApi->setTextSize)(0);
    (tftApi->setTextFont)(SMALL_FONT);
    (tftApi->setTextColor)(COLOR_BUTTON_TEXT, rectColor);
    (tftApi->drawString)(state->title, component->x + 4, component->y + 4);
}

bool buttonIsStateModified(Component *component)
{
    ButtonComponentState *state = (ButtonComponentState *)component->state;
    return (state->eventHandlingState != state->_eventHandlingState) || (state->mode != state->_mode);
}

void buttonUpdateState(Component *component)
{
    ButtonComponentState *state = (ButtonComponentState *)component->state;
    state->_eventHandlingState = state->eventHandlingState;
    state->_mode = state->mode;
}
