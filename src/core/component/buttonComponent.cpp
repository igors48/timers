#include "component.hpp"
#include "buttonComponent.hpp"

static void buttonComponentRender(Component *component, bool forced, TftApi *tftApi)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    unsigned int rectColor = COLOR_BLACK;
    if (state->pressed)
    {
        rectColor = COLOR_INTERACTION;
    }
    tftApi->drawRect(component->x, component->y, component->w, component->h, rectColor);
    tftApi->setCursor(component->x + 4, component->y + 4);
    tftApi->setTextSize(2);
    tftApi->setTextFont(1);
    tftApi->setTextColor(COLOR_INTERACTION, COLOR_BLACK);
    tftApi->print(state->title);
}

static bool buttonComponentNewState(Component *component)
{
    ButtonComponentState *state = (ButtonComponentState *)component->state;

    bool changed = false;

    if (state->pressed != state->_pressed)
    {
        changed = true;
        state->_pressed = state->pressed;
    }

    return changed;
}

static void buttonComponentOnTouch(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    state->pressed = true;
}

static void buttonComponentOnRelease(Component *component, signed short x, signed short y, unsigned int tickCount)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    state->handler();
    state->pressed = false;
}

ButtonComponentState createButtonState(char *title, Handler handler)
{
    return {
        .title = title,
        .pressed = false,
        ._pressed = true,
        .handler = handler,
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
        .onTouch = buttonComponentOnTouch,
        .onMove = componentNoopHandler,
        .onRelease = buttonComponentOnRelease,
        .onGesture = componentGestureNoopHandler,
        .render = buttonComponentRender,
        .newState = buttonComponentNewState,
        .state = state,
    };
}
