#include "tools/tools.hpp"

#include "stepCounterReset.hpp"

static const char STEP_COUNTER_RESET[] = "stepCounterReset";

void stepCounterReset(StepCounterResetParameters *p)
{
    if (p->systemApi->take(p->watchMutex, 1000))
    {
        Date now = p->rtcApi->getDate();
        bool periodStart = (now.hour == 21) && (now.minute == 0) && (now.second == 0);
        bool needReset = periodStart && (now.day != p->lastReset);
        if (needReset)
        {
            p->bmaApi->resetStepCounter();
            p->lastReset = now.day;
            p->systemApi->log(STEP_COUNTER_RESET, "step counter reset done");    
        }
        p->systemApi->give(p->watchMutex);
    }
    else
    {
        p->systemApi->log(STEP_COUNTER_RESET, "failed to take watch mutex");
    }
}

