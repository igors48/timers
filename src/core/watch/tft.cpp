#include "tft.hpp"

static TFT_eSprite *screeny;
static signed short cursorX; 
static signed short cursorY;

static void setCursor(signed short x, signed short y)
{
    //screeny->setCursor(x, y);
    cursorX = x;
    cursorY = y;
}

static void setTextSize(unsigned char s)
{
    screeny->setTextSize(s);
}

static void setTextFont(unsigned char f)
{
    screeny->setTextFont(f);
}

static void setTextColor(unsigned short f, unsigned short b)
{
    screeny->setTextColor(f, b);
}

static void print(const char str[])
{
    //screeny->print(str);
    screeny->drawString(str, cursorX, cursorY);
}

static void drawRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    screeny->drawRect(x, y, w, h, color);
}

static void fillRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    screeny->fillRect(x, y, w, h, color);
}

static void fillRoundRect(signed int x, signed int y, signed int w, signed int h, signed int r, unsigned int color)
{
    screeny->fillRoundRect(x, y, w, h, r, color);
}

static void drawFastVLine(signed int x, signed int y, signed int h, unsigned int color)
{
    screeny->drawFastVLine(x, y, h, color);
}

static void drawFastHLine(signed int x, signed int y, signed int w, unsigned int color)
{
    screeny->drawFastHLine(x, y, w, color);
}

static void pushSprite()
{
    screeny->pushSprite(0, 0);
}

TftApi watchTftApi(TFT_eSprite *screen)
{    
    screeny = screen;
    
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
        .pushSprite = pushSprite,
    };
}