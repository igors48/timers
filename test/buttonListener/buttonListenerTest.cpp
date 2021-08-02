#include <unity.h>

#include "../powerMock.hpp"
#include "../systemMock.hpp"

#include "task/buttonListener.cpp"

int watchMutex;
long lastUserEventTimestamp;
PowerApi powerApi;
SystemApi systemApi;

ButtonListenerParameters p;

long time()
{
    return 48;
}

bool notShortPress()
{
    return false;
}

void setUp(void)
{
    powerApi = powerApiMock();
    systemApi = systemApiMock();
    systemApi.time = time;

    lastUserEventTimestamp = 0;

    p = {
        .watchMutex = &watchMutex,
        .lastUserEventTimestamp = &lastUserEventTimestamp,
        .powerApi = &powerApi,
        .systemApi = &systemApi};
}

void whenItIsShortPress() 
{
    buttonListener(&p);

    TEST_ASSERT_EQUAL_UINT32(48, lastUserEventTimestamp); // THEN last user event timestamp changed
}

void whenItIsNotShortPress() 
{
    powerApi.isPEKShortPressIRQ = notShortPress;    

    buttonListener(&p);

    TEST_ASSERT_EQUAL_UINT32(0, lastUserEventTimestamp); // THEN last user event timestamp not changed
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenItIsShortPress);
    RUN_TEST(whenItIsNotShortPress);
    UNITY_END();
}