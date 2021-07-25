#pragma once

typedef struct
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} Date;

typedef Date (*DateFunc)();

typedef struct 
{
    DateFunc getDate;
} RtcApi;

RtcApi watchRtcApi();