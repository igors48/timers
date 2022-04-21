#pragma once

#include "core/system/system.hpp"
#include "core/watch/rtc.hpp"
#include "core/watch/bma.hpp"
#include "core/watch/motor.hpp"

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
