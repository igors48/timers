/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief RTC API implementation
*/
#include "rtc.hpp"
#include "ttgo.hpp"

/**
 * @copydoc RtcApi.getDate
 */
static Date getDate()
{
    RTC_Date rtcDate = watch->rtc->getDateTime();

    unsigned short year = rtcDate.year;
    unsigned char month = rtcDate.month;
    unsigned char day = rtcDate.day;
    unsigned char hour = rtcDate.hour;
    unsigned char minute = rtcDate.minute;
    unsigned char second = rtcDate.second;
    unsigned int dayOfWeek = watch->rtc->getDayOfWeek(day, month, year);

    return
    {
        .year = year,
        .month = month,
        .day = day,
        .hour = hour,
        .minute = minute,
        .second = second,
        .dayOfWeek = dayOfWeek,
    };
}

/**
 * @copydoc RtcApi.setDate
 */
static void setDate(Date date)
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
        .getDate = getDate,
        .setDate = setDate,
    };
}