#include <string.h>

#include "textComponent.hpp"

static void textComponentRender(Component *component, bool forced, WatchState *watchState, TftApi *tftApi)
{
    TextState *state = (TextState *)component->state;
    (tftApi->setCursor)(component->x, component->y);
    (tftApi->setTextSize)(state->size);
    (tftApi->setTextFont)(state->font);
    (tftApi->setTextColor)(state->fontColor, state->backColor);
    (tftApi->print)(state->content);
}

static bool textComponentNewState(Component *component, WatchState *watchState)
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

TextState createTextState(unsigned char font, unsigned char size, unsigned int fontColor, Provide provide)
{
    TextState textState = {};
    strcpy(textState.content, "");
    strcpy(textState.content, "_");
    textState.size = size;
    textState._size = 0;
    textState.fontColor = fontColor;
    textState._fontColor = 0;
    textState.backColor = COLOR_BLACK;
    textState._backColor = 0;
    textState.font = font;
    textState._font = 0;
    textState.provide = provide;
    return textState;
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