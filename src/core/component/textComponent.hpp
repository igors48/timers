#pragma once

#include "component.hpp"

typedef struct TextState_ TextState;

typedef void (*Provide)(TextState *state); 

const unsigned char CONTENT_LENGTH = 16; // todo increase or make it dynamic #66

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

void textComponentRender(Component *component, bool forced, TftApi *tftApi);
bool textComponentIsStateModified(Component *component);
void textComponentUpdateState(Component *component);
