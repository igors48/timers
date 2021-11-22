#include "component.hpp"
#include "buttonComponent.hpp"

void buttonComponentRender(Component *component, bool forced, WatchState *watchState, TftApi *tftApi)
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
    tftApi->print("Color");
}

bool buttonComponentNewState(Component *component, WatchState *watchState)
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

void buttonComponentOnTouch(Component *component, signed short x, signed short y)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    state->pressed = true;
}

void buttonComponentOnRelease(Component *component, signed short x, signed short y)
{
    ButtonComponentState *state = (ButtonComponentState *)(component->state);
    state->handler();
    state->pressed = false;
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
        .render = buttonComponentRender,
        .newState = buttonComponentNewState,
        .state = state,
    };
}
