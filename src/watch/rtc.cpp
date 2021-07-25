#include "rtc.hpp"
#include "ttgo.hpp"

Date rtcGetDate()
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

RtcApi watchRtcApi()
{
    return {
        .getDate = rtcGetDate};
}