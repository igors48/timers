#include "core/tools/tools.hpp"

#include "serviceProcedure.hpp"

static const char SERVICE_PROCEDURE[] = "serviceProcedure";

void serviceProcedure(ServiceProcedureParameters *p)
{
    if (p->systemApi->take(p->watchMutex, 1000))
    {
        Date now = p->rtcApi->getDate();
        bool hourStart = (now.minute == 0) && (now.second == 0);
        bool stepCounterPeriodStart = hourStart && (now.hour == 21); 
        bool needStepCounterReset = stepCounterPeriodStart && (now.day != p->lastStepCounterResetDay);
        bool buzzAllowedHour = now.hour >= 6 && now.hour <= 23;
        bool needHourBuzz = hourStart && buzzAllowedHour && (now.hour != p->lastBuzzHour);
        if (needHourBuzz)
        {
            // p->motorApi->buzz(100); temporarily disabled
            p->lastBuzzHour = now.hour;
            p->systemApi->log(SERVICE_PROCEDURE, "hour buzz");
        } 
        if (needStepCounterReset)
        {
            p->bmaApi->resetStepCounter();
            p->lastStepCounterResetDay = now.day;
            p->systemApi->log(SERVICE_PROCEDURE, "step counter reset done");    
        }
        p->systemApi->give(p->watchMutex);
    }
    else
    {
        p->systemApi->log(SERVICE_PROCEDURE, "failed to take watch mutex");
    }
}

