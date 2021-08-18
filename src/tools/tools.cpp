#include <time.h>
#include <stdio.h>

#include "tools.hpp"

unsigned long secondsToNextHourStart(Date now)
{
    unsigned char seconds = now.second;
    unsigned char minutes = now.minute;
    if (seconds == 0 && minutes == 0) {
        return 0;
    }
    seconds = 60 - seconds;
    minutes = 59 - minutes;
    return minutes * 60 + seconds;        
}