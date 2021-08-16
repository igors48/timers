#include "stepCounterReset.hpp"

static const char STEP_COUNTER_RESET[] = "stepCounterReset";

StepCounterResetResult _stepCounterReset(StepCounterResetParameters *p)
{
    if (p->systemApi->take(p->watchMutex, 10))
    {
        Date date = p->rtcApi->getDate();
        bool startOfTheHour = (date.minute == 0) && (date.second == 0);
        if (startOfTheHour)
        {
            bool success = p->bmaApi->resetStepCounter();
            if (success)
            {
                return SUCCESS;
            } 
            else
            {
                return FAIL_ON_MUTEX;
            }
        } else {
            return OUTDATED;
        }
    }
    else
    {
        p->systemApi->log(STEP_COUNTER_RESET, "failed to take watch mutex");
        return FAIL_ON_MUTEX;
    }
}

void stepCounterReset(StepCounterResetParameters *p)
{
    StepCounterResetResult result = _stepCounterReset(p);
}