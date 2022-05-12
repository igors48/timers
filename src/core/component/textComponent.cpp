#include <string.h>

#include <LilyGoWatch.h>

#include "textComponent.hpp"

static void textComponentRender(Component *component, bool forced, TftApi *tftApi)
{
    TextState *state = (TextState *)component->state;
    (tftApi->setCursor)(component->x, component->y);
    (tftApi->setTextSize)(state->size);
    (tftApi->setTextFont)(state->font);
    (tftApi->setTextColor)(state->fontColor, state->backColor);
    (tftApi->print)(state->content);
}

static bool textComponentNewState(Component *component)
{
    TextState *state = (TextState *)component->state;
    (state->provide)(state);
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

TextState* createTextStateRef(unsigned char font, unsigned char size, unsigned int fontColor, Provide provide)
{
    TextState *ref = (TextState *)pvPortMalloc(sizeof(TextState));

    strcpy(ref->content, "");
    strcpy(ref->_content, "_");
    ref->size = size;
    ref->_size = 0;
    ref->fontColor = fontColor;
    ref->_fontColor = 0;
    ref->backColor = COLOR_BLACK;
    ref->_backColor = 0;
    ref->font = font;
    ref->_font = 0;
    ref->provide = provide;

    return ref;
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
        .onGesture = componentGestureNoopHandler,
        .render = textComponentRender,
        .newState = textComponentNewState,
        .state = state,
    };
}

Component* createTextComponentRef(signed short x, signed short y, signed short w, signed short h, TextState *state)
{
    Component* ref = (Component *)pvPortMalloc(sizeof(Component));

    ref->x = x;
    ref->y = y;
    ref->w = w;
    ref->h = h;
    ref->contains = componentContains;
    ref->mount = componentMount;
    ref->onTouch = componentNoopHandler;
    ref->onMove = componentNoopHandler;
    ref->onRelease = componentNoopHandler;
    ref->onGesture = componentGestureNoopHandler;
    ref->render = textComponentRender;
    ref->newState = textComponentNewState;
    ref->state = state;

    return ref;
}