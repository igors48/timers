#include <string.h>

#include "textComponent.hpp"

void textComponentRender(Component *component, bool forced, TftApi *tftApi)
{
    TextState *state = (TextState *)component->state;
    (tftApi->setTextSize)(state->size);
    (tftApi->setTextFont)(state->font);
    (tftApi->setTextColor)(state->fontColor, state->backColor);
    (tftApi->drawString)(state->content, component->x, component->y);
}

bool textComponentIsStateModified(Component *component)
{
    TextState *state = (TextState *)component->state;
    (state->provide)(state);
    return (strcmp(state->content, state->_content) != 0) ||
    (state->size != state->_size) ||
    (state->fontColor != state->_fontColor) ||
    (state->backColor != state->_backColor) ||
    (state->font != state->_font);
}

void textComponentUpdateState(Component *component)
{
    TextState *state = (TextState *)component->state;
    strcpy(state->_content, state->content);
    state->_size = state->size;
    state->_fontColor = state->fontColor;
    state->_backColor = state->backColor;
    state->_font = state->font;
}
