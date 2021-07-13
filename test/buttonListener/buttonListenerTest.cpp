#include <unity.h>

#include "watch/power.hpp"
#include "system/system.hpp"

#include "../powerMock.hpp"
#include "../systemMock.hpp"

#include "task/buttonListener.cpp"

int lastShortPressTimestampMutex;
long lastShortPressTimestamp;
PowerApi powerApi;
SystemApi systemApi;

ButtonListenerParameters p;

long time()
{
    return 48;
}

void setUp(void)
{
    powerApi = powerApiMock();
    systemApi = systemApiMock();
    systemApi.time = time;

    lastShortPressTimestamp = 0;

    p = {
        .lastShortPressTimestampMutex = &lastShortPressTimestampMutex,
        .lastShortPressTimestamp = &lastShortPressTimestamp,
        .powerApi = &powerApi,
        .systemApi = &systemApi};
}

void smoke() 
{
    buttonListener(&p);
    TEST_ASSERT_EQUAL_UINT32(48, lastShortPressTimestamp);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(smoke);
    UNITY_END();
}