#include <unity.h>

#include "watch/power.hpp"
#include "system/system.hpp"

#include "task/buttonListener.hpp"

void lastShortPressTimestampMutex;
long lastShortPressTimestamp;
PowerApi powerApi;
SystemApi systemApi;

ButtonListenerParameters p;

void emptyVoid()
{
}

bool boolTrue()
{
    return true;
}

bool boolFalse()
{
    return false;
}

/*
typedef struct
{
    void *lastShortPressTimestampMutex;
    long *lastShortPressTimestamp;
    PowerApi *powerApi;
    SystemApi *systemApi;
} ButtonListenerParameters;
*/

void setUp(void)
{
    powerApi = {
    .readIRQ = emptyVoid,
    .isPEKShortPressIRQ = boolTrue,
    .clearIRQ = emptyVoid};

    systemApi = {
        .take = systemTake,
        .give = systemGive,
        .log = systemLog,
        .time = systemTime};
}

int main()
{
    UNITY_BEGIN();
    //RUN_TEST(whenActionModeAndIdleTimePassed);
    UNITY_END();
}