/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief TFT API definition
 * 
 * List of available functions for work with TFT   
*/
#pragma once

typedef struct
{
        void (*setTextSize)(unsigned char s);
        void (*setTextFont)(unsigned char f);
        void (*setTextColor)(unsigned short f, unsigned short b);
        void (*setTextColorOnly)(unsigned short f);
        void (*drawString)(const char str[], signed int x, signed int y);
        void (*drawRect)(signed int x, signed int y, signed int w, signed int h, unsigned int color);
        void (*fillRect)(signed int x, signed int y, signed int w, signed int h, unsigned int color);
        void (*fillRoundRect)(signed int x, signed int y, signed int w, signed int h, signed int r, unsigned int color);
        void (*drawFastVLine)(signed int x, signed int y, signed int h, unsigned int color);
        void (*drawFastHLine)(signed int x, signed int y, signed int w, unsigned int color);
        void (*pushSprite)();
} TftApi;

TftApi watchTftApi();