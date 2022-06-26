#pragma once

typedef struct
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned int dayOfWeek;
} Date;

typedef Date (*DateFunc)(); // todo rename to GetDate
typedef void (*SetDate)(Date date);

typedef struct 
{
    DateFunc getDate;
    SetDate setDate;
} RtcApi;

RtcApi watchRtcApi();