/**
 * @file
 * @author Igor Usenko (github: igors48)
 * @brief TFT API implementation based on sprite
 *
 * Holds in memory full-screen sprite.
 * <br>
 * All drawings are performed on that sprite.
 * When all is drawn - sprite is pushed to video memory.
 * <br>
 * Also maintains the dirty flag to prevent not needed pushes.
 */
#include "tft.hpp"
#include "ttgo.hpp"
#include "smallFont.hpp"
#include "mediumFont.hpp"

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
    if (f == SMALL_FONT)
    {
        sprite->setFreeFont(&Roboto_Condensed_Bold_16);
        return;
    }
    if (f == MEDIUM_FONT)
    {
       sprite->setFreeFont(&Roboto_Condensed_Bold_24);
       return;
    }
    sprite->setTextFont(f);
}

static void setTextColor(unsigned short f, unsigned short b)
{
    dirty = true;
    sprite->setTextColor(f, b);
}

static void setTextColorOnly(unsigned short f)
{
    dirty = true;
    sprite->setTextColor(f);
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
        .setTextColorOnly = setTextColorOnly,
        .drawString = drawString,
        .drawRect = drawRect,
        .fillRect = fillRect,
        .fillRoundRect = fillRoundRect,
        .drawFastVLine = drawFastVLine,
        .drawFastHLine = drawFastHLine,
        .pushSprite = pushSprite,
    };
}