#include "watchState.hpp"

WatchState copy(WatchState watchState)
{
    Date origin = watchState.date;
    Date date = {.year = origin.year,
                 .month = origin.month,
                 .day = origin.day,
                 .hour = origin.hour,
                 .minute = origin.minute,
                 .second = origin.second};

    return {
        .date = date
    };                 
}