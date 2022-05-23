#include <unity.h>

#include "../powerMock.hpp"
#include "../systemMock.hpp"

#include "core/task/buttonListener.cpp"

int watchMutex;
long lastUserEventTimestamp;
PowerApi powerApi;
SystemApi systemApi;
Tiler tiler;
bool onButtonCalled;

ButtonListenerParameters p;

long time()
{
    return 48;
}

bool notShortPress()
{
    return false;
}

void onButton()
{
    onButtonCalled = true;
}

void setUp(void)
{
    powerApi = powerApiMock();
    systemApi = systemApiMock();
    systemApi.time = time;

    onButtonCalled = false;
    tiler.onButton = onButton;

    lastUserEventTimestamp = 0;

    p = {
        .watchMutex = &watchMutex,
        .lastUserEventTimestamp = &lastUserEventTimestamp,
        .tiler = &tiler,
        .powerApi = &powerApi,
        .systemApi = &systemApi};
}

void whenItIsShortPress() 
{
    buttonListener(&p);

    TEST_ASSERT_EQUAL_UINT32(48, lastUserEventTimestamp); // THEN last user event timestamp changed
    TEST_ASSERT_TRUE(onButtonCalled); // THEN button handler called
}

void whenItIsNotShortPress() 
{
    powerApi.isPEKShortPressIRQ = notShortPress;    

    buttonListener(&p);

    TEST_ASSERT_EQUAL_UINT32(0, lastUserEventTimestamp); // THEN last user event timestamp not changed
    TEST_ASSERT_FALSE(onButtonCalled); // THEN button handler not called
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenItIsShortPress);
    RUN_TEST(whenItIsNotShortPress);
    UNITY_END();
}