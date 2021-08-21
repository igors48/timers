#pragma once

#include "system/system.hpp"
#include "watch/rtc.hpp"
#include "watch/bma.hpp"
#include "watch/motor.hpp"

typedef struct 
{
    void *watchMutex;
    unsigned char lastBuzzHour;
    unsigned char lastStepCounterResetDay;
    RtcApi *rtcApi;
    BmaApi *bmaApi;
    SystemApi *systemApi;
    MotorApi *motorApi;
} ServiceProcedureParameters;

void serviceProcedure(ServiceProcedureParameters *p);
