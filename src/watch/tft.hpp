#pragma once

typedef void (*SetCursor)(signed short x, signed short y);
typedef void (*SetTextSize)(unsigned char s);
typedef void (*SetTextFont)(unsigned char f);
typedef void (*SetTextColor)(unsigned short f, unsigned short b);
typedef void (*PrintFunc)(const char str[]);
typedef void (*DrawRect)(signed int x, signed int y, signed int w, signed int h, unsigned int color);
typedef void (*FillRect)(signed int x, signed int y, signed int w, signed int h, unsigned int color);

typedef struct
{
        SetCursor setCursor;
        SetTextSize setTextSize;
        SetTextFont setTextFont;
        SetTextColor setTextColor;
        PrintFunc print;
        DrawRect drawRect;
        FillRect fillRect;
} TftApi;

TftApi watchTftApi();