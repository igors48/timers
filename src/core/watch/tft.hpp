#pragma once

#include "ttgo.hpp"

typedef struct
{
        void (*setCursor)(signed short x, signed short y);
        void (*setTextSize)(unsigned char s);
        void (*setTextFont)(unsigned char f);
        void (*setTextColor)(unsigned short f, unsigned short b);
        void (*print)(const char str[]);
        void (*drawRect)(signed int x, signed int y, signed int w, signed int h, unsigned int color);
        void (*fillRect)(signed int x, signed int y, signed int w, signed int h, unsigned int color);
        void (*fillRoundRect)(signed int x, signed int y, signed int w, signed int h, signed int r, unsigned int color);
        void (*drawFastVLine)(signed int x, signed int y, signed int h, unsigned int color);
        void (*drawFastHLine)(signed int x, signed int y, signed int w, unsigned int color);
        void (*pushSprite)();
} TftApi;

TftApi watchTftApi(TFT_eSprite *screen);