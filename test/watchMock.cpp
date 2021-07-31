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

bool getTouchMock(signed short &x, signed short &y)
{
    return false;
}

WatchApi watchApiMock()
{
    return {
        .init = initMock,
        .afterWakeUp = afterWakeUpMock,
        .beforeGoToSleep = beforeGoToSleepMock,
        .goToSleep = goToSleepMock,
        .getTouch = getTouchMock,
    };
}