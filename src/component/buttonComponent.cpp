#include "component.hpp"
#include "buttonComponent.hpp"

void buttonComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextSize(2);
    tftApi->setTextFont(1);
    tftApi->setTextColor(0xFDA0, 0x0000);
    tftApi->print("Color");
}

void buttonComponentOnTouch(Component component)
{
    ButtonComponentState *state = (ButtonComponentState *)component.state;
    state->handler();
}

Component createButtonComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, ButtonComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .state = state,
        .render = buttonComponentRender,
        .onTouch = buttonComponentOnTouch,
        .newState = componentNewState,
    };
}
