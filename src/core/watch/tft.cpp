#include "tft.hpp"

static TFT_eSprite *sprite;

static void setTextSize(unsigned char s)
{
    sprite->setTextSize(s);
}

static void setTextFont(unsigned char f)
{
    sprite->setTextFont(f);
}

static void setTextColor(unsigned short f, unsigned short b)
{
    sprite->setTextColor(f, b);
}

static void drawString(const char str[], signed int x, signed int y)
{
    sprite->drawString(str, x, y);
}

static void drawRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    sprite->drawRect(x, y, w, h, color);
}

static void fillRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    sprite->fillRect(x, y, w, h, color);
}

static void fillRoundRect(signed int x, signed int y, signed int w, signed int h, signed int r, unsigned int color)
{
    sprite->fillRoundRect(x, y, w, h, r, color);
}

static void drawFastVLine(signed int x, signed int y, signed int h, unsigned int color)
{
    sprite->drawFastVLine(x, y, h, color);
}

static void drawFastHLine(signed int x, signed int y, signed int w, unsigned int color)
{
    sprite->drawFastHLine(x, y, w, color);
}

static void pushSprite()
{
    sprite->pushSprite(0, 0);
}

TftApi watchTftApi()
{
    sprite = new TFT_eSprite(watch->tft);
    sprite->createSprite(240, 240);

    return {
        .setTextSize = setTextSize,
        .setTextFont = setTextFont,
        .setTextColor = setTextColor,
        .drawString = drawString,
        .drawRect = drawRect,
        .fillRect = fillRect,
        .fillRoundRect = fillRoundRect,
        .drawFastVLine = drawFastVLine,
        .drawFastHLine = drawFastHLine,
        .pushSprite = pushSprite,
    };
}