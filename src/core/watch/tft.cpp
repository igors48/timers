#include "tft.hpp"
#include "ttgo.hpp"

static void setCursor(signed short x, signed short y)
{
    watch->tft->setCursor(x, y);
}

static void setTextSize(unsigned char s)
{
    watch->tft->setTextSize(s);
}

static void setTextFont(unsigned char f)
{
    watch->tft->setTextFont(f);
}

static void setTextColor(unsigned short f, unsigned short b)
{
    watch->tft->setTextColor(f, b);
}

static void print(const char str[])
{
    watch->tft->print(str);
}

static void drawRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    watch->tft->drawRect(x, y, w, h, color);
}

static void fillRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    watch->tft->fillRect(x, y, w, h, color);
}

static void fillRoundRect(signed int x, signed int y, signed int w, signed int h, signed int r, unsigned int color)
{
    watch->tft->fillRoundRect(x, y, w, h, r, color);
}

static void drawFastVLine(signed int x, signed int y, signed int h, unsigned int color)
{
    watch->tft->drawFastVLine(x, y, h, color);
}

static void drawFastHLine(signed int x, signed int y, signed int w, unsigned int color)
{
    watch->tft->drawFastHLine(x, y, w, color);
}

TftApi watchTftApi()
{
    return {
        .setCursor = setCursor,
        .setTextSize = setTextSize,
        .setTextFont = setTextFont,
        .setTextColor = setTextColor,
        .print = print,
        .drawRect = drawRect,
        .fillRect = fillRect,
        .fillRoundRect = fillRoundRect,
        .drawFastVLine = drawFastVLine,
        .drawFastHLine = drawFastHLine,
    };
}