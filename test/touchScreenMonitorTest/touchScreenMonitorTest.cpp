#include <stdarg.h>
#include <cstdio>
#include <unity.h>

#include "touchScreenMonitor.cpp"

int logCount;
bool giveResult;
bool takeResult;
long timeResult;
bool getTouchedResult;

int lastTouchTimestampMutex;
long lastTouchTimestamp;

TouchScreenMonitorParameters parameters;

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

bool getTouched() {
    return getTouchedResult;
}

void setUp(void)
{
    logCount = 0;
    giveResult = true;
    takeResult = true;
    timeResult = 0;
    getTouchedResult = false;

    parameters = {
    .lastTouchTimestampMutex = &lastTouchTimestampMutex,
    .lastTouchTimestamp = &lastTouchTimestamp,
    .getTouched = getTouched,
    .time = time,
    .take = take,
    .give = give,
    .log = log};
}

void whenScreenNotTouched() {
    touchScreenMonitor(&parameters);
    TEST_ASSERT_EQUAL_UINT32(0, lastTouchTimestamp); // last touch time stamp does not change
}

void whenScreenTouched() {
    lastTouchTimestamp = 48;
    touchScreenMonitor(&parameters);
    TEST_ASSERT_EQUAL_UINT32(48, lastTouchTimestamp); // last touch time stamp changed
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenScreenNotTouched);
    RUN_TEST(whenScreenTouched);
    UNITY_END();
}