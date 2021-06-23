#include <stdarg.h>
#include <cstdio>
#include <unity.h>
#include "supervisor.cpp"

/*
typedef struct
{
    void *actionMutex;
    bool *action;
    void *lastEventTimestampMutex;
    long *lastEventTimestamp;
    long wakeUpTime;
    Func wakeUp;
    long goToSleepTime;
    Func goToSleep;
    Time time;
    Take take;
    Give give;
    Log log;
} SupervisorParameters;
*/

int main()
{
    UNITY_BEGIN();
    //RUN_TEST(whenTimeSinceLastTouchLesserThan5Sec);
    UNITY_END();
}