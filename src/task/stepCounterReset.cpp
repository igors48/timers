#include "tools/tools.hpp"

#include "stepCounterReset.hpp"

static const char STEP_COUNTER_RESET[] = "stepCounterReset";

void stepCounterReset(StepCounterResetParameters *p)
{
    if (p->systemApi->take(p->watchMutex, 1000))
    {
        Date now = p->rtcApi->getDate();
        bool hourStart = (now.minute == 0) && (now.second == 0);
        bool needReset = hourStart && (now.hour != p->lastReset);
        if (needReset)
        {
            p->bmaApi->resetStepCounter();
            p->lastReset = now.hour;
            p->systemApi->log(STEP_COUNTER_RESET, "step counter reset done");    
        }
        p->systemApi->give(p->watchMutex);
    }
    else
    {
        p->systemApi->log(STEP_COUNTER_RESET, "failed to take watch mutex");
    }
}

