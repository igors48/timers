#include <string.h>

#include "textComponent.hpp"

void textComponentRender(Component *component, bool forced, TftApi *tftApi)
{
    TextState *state = (TextState *)component->state;
    (tftApi->setCursor)(component->x, component->y);
    (tftApi->setTextSize)(state->size);
    (tftApi->setTextFont)(state->font);
    (tftApi->setTextColor)(state->fontColor, state->backColor);
    (tftApi->print)(state->content);
}

bool textComponentNewState(Component *component)
{
    TextState *state = (TextState *)component->state;
    (state->provide)(state);
    if (strcmp(state->content, state->_content) != 0) { // todo update prev state _* variables
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
