#include <stdarg.h>
#include <cstdio>
#include <unity.h>
#include "noEventsMonitor.cpp"

int logCount;
bool giveResult;
bool takeResult;
long timeResult;
bool lightSleepCalled;

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

void lightSleep() {
    lightSleepCalled = true;
}

void setUp(void)
{
    logCount = 0;
    giveResult = true;
    takeResult = true;
    timeResult = 0;
    lightSleepCalled = false;

    parameters = {
    .lastTouchTimestampMutex = &lastTouchTimestampMutex,
    .lastTouchTimestamp = &lastTouchTimestamp,
    .backlightLevelMutex = &backlightLevelMutex,
    .backlightLevel = &backlightLevel,
    .lightSleep = lightSleep,
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
    TEST_ASSERT_EQUAL_UINT8(0, lightSleepCalled); // lightSleep not called
}

void whenTimeSinceLastTouchBiggerThan5Sec()
{
    lastTouchTimestamp = 5;
    timeResult = 12;
    noEventsMonitor(&parameters);
    TEST_ASSERT_EQUAL_UINT8(8, backlightLevel); // backlight level set to min
    TEST_ASSERT_EQUAL_UINT8(0, lightSleepCalled); // lightSleep not called
}

void whenTimeSinceLastTouchBiggerThan10Sec()
{
    lastTouchTimestamp = 5;
    timeResult = 21;
    noEventsMonitor(&parameters);
    TEST_ASSERT_EQUAL_UINT8(1, lightSleepCalled); // lightSleep not called
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenTimeSinceLastTouchLesserThan5Sec);
    RUN_TEST(whenTimeSinceLastTouchBiggerThan5Sec);
    RUN_TEST(whenTimeSinceLastTouchBiggerThan10Sec);
    UNITY_END();
}