#include <stdarg.h>
#include <cstdio>
#include <unity.h>
#include "noEventsMonitor.cpp"

int logCount;
bool giveResult;
bool takeResult;
long timeResult;

int lastTouchTimestampMutex;
long lastTouchTimestamp;
int backlightLevelMutex;
unsigned char backlightLevel;

NoEventsMonitorParameters parameters;

/*
typedef struct
{
    void *lastTouchTimestampMutex;
    long *lastTouchTimestamp;
    void *backlightLevelMutex;
    unsigned char *backlightLevel;
    Take take;
    Give give;
    Time time;
    Log log;

} NoEventsMonitorParameters;
*/
void log(const char *source, const char *message, ...)
{
    logCount++;
    char buf[256];
    va_list args;
    va_start(args, message);
    vsprintf(buf, message, args);
    va_end(args);
    printf("%s: %s \r\n", source, buf);
}

bool give(void *semaphore)
{
    return giveResult;
}

bool take(void *semaphore, unsigned int blockTime)
{
    return takeResult;
}

long time()
{
    return timeResult;
}

void setUp(void)
{
    logCount = 0;
    giveResult = true;
    takeResult = true;
    timeResult = 0;

    parameters = {
    .lastTouchTimestampMutex = &lastTouchTimestampMutex,
    .lastTouchTimestamp = &lastTouchTimestamp,
    .backlightLevelMutex = &backlightLevelMutex,
    .backlightLevel = &backlightLevel,
    .take = take,
    .give = give,
    .time = time,
    .log = log};
}

void happyFlow()
{
    noEventsMonitor(&parameters);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(happyFlow);
    UNITY_END();
}