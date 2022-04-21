#include "tft.hpp"
#include "ttgo.hpp"

void tftSetCursor(signed short x, signed short y)
{
    watch->tft->setCursor(x, y);
}

void tftSetTextSize(unsigned char s)
{
    watch->tft->setTextSize(s);
}

void tftSetTextFont(unsigned char f)
{
    watch->tft->setTextFont(f);
}

void tftSetTextColor(unsigned short f, unsigned short b)
{
    watch->tft->setTextColor(f, b);
}

void tftPrint(const char str[])
{
    watch->tft->print(str);
}

void tftDrawRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    watch->tft->drawRect(x, y, w, h, color);
}

void tftFillRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    watch->tft->fillRect(x, y, w, h, color);
}

TftApi watchTftApi()
{
    return {
        .setCursor = tftSetCursor,
        .setTextSize = tftSetTextSize,
        .setTextFont = tftSetTextFont,
        .setTextColor = tftSetTextColor,
        .print = tftPrint,
        .drawRect = tftDrawRect,
        .fillRect = tftFillRect,
    };
}