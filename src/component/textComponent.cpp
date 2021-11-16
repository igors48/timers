#include <string.h>

#include "textComponent.hpp"

void textComponentRender(Component *component, WatchState *watchState, TftApi *tftApi)
{
    TextState *state = (TextState *)component->state;
    (tftApi->setCursor)(component->x, component->y);
    (tftApi->setTextSize)(state->size);
    (tftApi->setTextFont)(state->font);
    (tftApi->setTextColor)(state->fontColor, state->backColor);
    (tftApi->print)(state->content);
}

bool textComponentNewState(Component *component, WatchState *watchState)
{
    TextState *state = (TextState *)component->state;
    (state->provide)(state, watchState);
    if (strcmp(state->content, state->_content) != 0) {
        return true;
    }
    if (state->size != state->_size) {
        return true;
    }
    if (state->fontColor != state->_fontColor) {
        return true;
    }
    if (state->backColor != state->_backColor) {
        return true;
    }
    if (state->font != state->_font) {
        return true;
    }
    return false;
}

TextState createTextState(unsigned char font, unsigned char size, Provide provide)
{
    TextState s = {};
    strcpy(s.content, "");
    strcpy(s.content, "_");
    s.size = size;
    s._size = 0;
    s.fontColor = COLOR_INFORMATION;
    s._fontColor = 0;
    s.backColor = COLOR_BLACK;
    s._backColor = 0;
    s.font = font;
    s._font = 0;
    s.provide = provide;
    return s;
}

Component createTextComponent(signed short x, signed short y, signed short w, signed short h, TextState *state)
{
    return {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .contains = componentContains,
        .mount = componentMount,
        .onTouch = componentNoopHandler,
        .onMove = componentNoopHandler,
        .onRelease = componentNoopHandler,
        .render = textComponentRender,
        .newState = textComponentNewState,
        .state = state,
    };
}