/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief RTC API definition
*/
#pragma once

/**
 * @brief definition of Date
 */
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

typedef struct 
{
    /**
     * @brief returns the date from RTC
     * 
     * @return date value
     */
    Date (*getDate)();

    /**
     * @brief writes the date to RTC
     * 
     * @param date date value
     */
    void (*setDate)(Date date);
} RtcApi;

/**
 * @brief factory for RTC API
 * 
 * @return RtcApi RTC API instance
 */
RtcApi watchRtcApi();