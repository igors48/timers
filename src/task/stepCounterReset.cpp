#include "tools/tools.hpp"

#include "stepCounterReset.hpp"

static const char STEP_COUNTER_RESET[] = "stepCounterReset";

void stepCounterReset(StepCounterResetParameters *p)
{
    if (p->systemApi->take(p->watchMutex, 1000))
    {
        p->bmaApi->resetStepCounter();
        Date now = p->rtcApi->getDate();
        unsigned long nextExecution = secondsToNextHourStart(now) * 1000;
        //unsigned long nextExecution = 30000;
        p->systemApi->give(p->watchMutex);
        p->systemApi->log(STEP_COUNTER_RESET, "lastWakeTime %ld, nextExecution %ld", *(p->lastWakeTime), nextExecution);
        p->systemApi->delayUntil(p->lastWakeTime, nextExecution);
    }
    else
    {
        p->systemApi->log(STEP_COUNTER_RESET, "failed to take watch mutex");
    }
}

