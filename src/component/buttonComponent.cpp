#include "component.hpp"
#include "buttonComponent.hpp"

void buttonComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    ButtonComponentState *state = (ButtonComponentState *)(component.state);
    unsigned int rectColor = 0x0000;
    if (state->pressed)
    {
        rectColor = 0x07E0;
    }
    tftApi->drawRect(component.x, component.y, component.w, component.h, rectColor);
    tftApi->setCursor(component.x + 4, component.y + 4);
    tftApi->setTextSize(2);
    tftApi->setTextFont(1);
    tftApi->setTextColor(0x07E0, 0x0000);
    tftApi->print("Color");
}

bool buttonComponentNewState(Component component, WatchState watchState)
{
    ButtonComponentState *state = (ButtonComponentState *)component.state;

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
        .onTouch = buttonComponentOnTouch,
        .onMove = componentNoopHandler,
        .onRelease = buttonComponentOnRelease,
        .render = buttonComponentRender,
        .newState = buttonComponentNewState,
        .state = state,
    };
}
