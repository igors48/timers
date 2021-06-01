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

void whenTimeSinceLastTouchLesserThan5Sec()
{
    lastTouchTimestamp = 5;
    timeResult = 7;
    noEventsMonitor(&parameters);
    TEST_ASSERT_EQUAL_UINT8(128, backlightLevel); // backlight level set to max
}

void whenTimeSinceLastTouchBiggerThan5Sec()
{
    lastTouchTimestamp = 5;
    timeResult = 18;
    noEventsMonitor(&parameters);
    TEST_ASSERT_EQUAL_UINT8(8, backlightLevel); // backlight level set to min
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTimeSinceLastTouchLesserThan5Sec);
    RUN_TEST(whenTimeSinceLastTouchBiggerThan5Sec);
    UNITY_END();
}