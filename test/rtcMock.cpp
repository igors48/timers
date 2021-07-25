#include "rtcMock.hpp"

Date getDateMock()
{
    return
    {
        .year = 1,
        .month = 2,
        .day = 3,
        .hour = 4,
        .minute = 5,
        .second = 6,
    };
}

RtcApi rtcApiMock()
{
    return {
        .getDate = getDateMock};
}
