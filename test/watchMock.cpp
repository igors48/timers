#include "watchMock.hpp"

void initMock()
{
    // empty
}

void afterWakeUpMock()
{
    // empty
}

void beforeGoToSleepMock()
{
    // empty
}

void goToSleepMock()
{
    // empty
}

WatchApi watchApiMock()
{
    return {
        .init = initMock,
        .afterWakeUp = afterWakeUpMock,
        .beforeGoToSleep = beforeGoToSleepMock,
        .goToSleep = goToSleepMock
    };
}