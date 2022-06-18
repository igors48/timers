#include "tft.hpp"
#include "ttgo.hpp"

static TFT_eSprite *sprite;
static bool dirty;

static void setTextSize(unsigned char s)
{
    dirty = true;
    sprite->setTextSize(s);
}

static void setTextFont(unsigned char f)
{
    dirty = true;
    sprite->setTextFont(f);
}

static void setTextColor(unsigned short f, unsigned short b)
{
    dirty = true;
    sprite->setTextColor(f, b);
}

static void drawString(const char str[], signed int x, signed int y)
{
    dirty = true;
    sprite->drawString(str, x, y);
}

static void drawRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    dirty = true;
    sprite->drawRect(x, y, w, h, color);
}

static void fillRect(signed int x, signed int y, signed int w, signed int h, unsigned int color)
{
    dirty = true;
    sprite->fillRect(x, y, w, h, color);
}

static void fillRoundRect(signed int x, signed int y, signed int w, signed int h, signed int r, unsigned int color)
{
    dirty = true;
    sprite->fillRoundRect(x, y, w, h, r, color);
}

static void drawFastVLine(signed int x, signed int y, signed int h, unsigned int color)
{
    dirty = true;
    sprite->drawFastVLine(x, y, h, color);
}

static void drawFastHLine(signed int x, signed int y, signed int w, unsigned int color)
{
    dirty = true;
    sprite->drawFastHLine(x, y, w, color);
}

static void pushSprite()
{
    if (dirty)
    {
        sprite->pushSprite(0, 0);
    }
    dirty = false;
}

TftApi watchTftApi()
{
    sprite = new TFT_eSprite(watch->tft);
    sprite->createSprite(240, 240);

    dirty = false;
    
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