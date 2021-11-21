#pragma once

#include "component.hpp"

typedef struct TextState_ TextState;

// todo return boolean "dirty" flag and reduce logic in the newState
// todo or even just pass newState function as a parameter
typedef void (*Provide)(TextState *state, WatchState *watchState); 

const unsigned char CONTENT_LENGTH = 16;

struct TextState_
{
    char content[CONTENT_LENGTH];
    char _content[CONTENT_LENGTH];
    unsigned char size;
    unsigned char _size;
    unsigned int fontColor;
    unsigned int _fontColor;
    unsigned int backColor;
    unsigned int _backColor;
    unsigned char font;
    unsigned char _font;
    Provide provide;
};

TextState createTextState(unsigned char font, unsigned char size, unsigned int fontColor, Provide provide);

Component createTextComponent(signed short x, signed short y, signed short w, signed short h, TextState *state);