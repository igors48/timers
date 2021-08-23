#include "component.hpp"
#include "changeColorComponent.hpp"

void changeColorComponentRender(Component component, WatchState watchState, TftApi *tftApi)
{
    tftApi->setCursor(component.x, component.y);
    tftApi->setTextSize(2);
    tftApi->setTextFont(1);
    tftApi->setTextColor(0xFDA0, 0x0000);
    tftApi->print("Color");
}

void changeColorComponentOnTouch(Component component)
{
    ChangeColorComponentState *state = (ChangeColorComponentState *)component.state;
    state->handler();
}

Component createChangeColorComponent(unsigned char x, unsigned char y, unsigned char w, unsigned char h, ChangeColorComponentState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .state = state,
        .render = changeColorComponentRender,
        .onTouch = changeColorComponentOnTouch,
        .newState = componentNewState,
    };
}
