#pragma once

typedef void (*SetCursor)(signed short x, signed short y);
typedef void (*SetTextSize)(unsigned char s);
typedef void (*SetTextFont)(unsigned char f);
typedef void (*PrintFunc)(const char str[]);

typedef struct
{
        SetCursor setCursor;
        SetTextSize setTextSize;
        SetTextFont setTextFont;
        PrintFunc print;
} TftApi;

TftApi watchTftApi();