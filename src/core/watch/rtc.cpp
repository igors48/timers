#include "rtc.hpp"
#include "ttgo.hpp"

static Date rtcGetDate()
{
    RTC_Date rtcDate = watch->rtc->getDateTime();

    unsigned short year = rtcDate.year;
    unsigned char month = rtcDate.month;
    unsigned char day = rtcDate.day;
    unsigned char hour = rtcDate.hour;
    unsigned char minute = rtcDate.minute;
    unsigned char second = rtcDate.second;

    return
    {
        .year = year,
        .month = month,
        .day = day,
        .hour = hour,
        .minute = minute,
        .second = second
    };
}

static void rtcSetDate(Date date)
{
    RTC_Date rtcDate = RTC_Date(
        date.year,
        date.month,
        date.day,
        date.hour,
        date.minute,
        date.second        
    );
    watch->rtc->setDateTime(rtcDate);
}

RtcApi watchRtcApi()
{
    return {
        .getDate = rtcGetDate,
        .setDate = rtcSetDate,
    };
}