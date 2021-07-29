#include "watchState.hpp"

WatchState initialWatchState()
{
    Date date = {
        .year = 0,
        .month = 0,
        .day = 0,
        .hour = 0,
        .minute = 0,
        .second = 0,
    };

    return {
        .date = date,
        .battPercentage = 0
    };
}