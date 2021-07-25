#include "systemMock.hpp"

bool takeMock(void *semaphore, unsigned int blockTime)
{
    return true;
}

bool giveMock(void *semaphore)
{
    return true;
}

long timeMock()
{
    return 0;
}

void logMock(const char *source, const char *message, ...)
{
    // empty
}

void delayMock(unsigned int time)
{
    // empty
}

void suspendMock(void *handle)
{
    // empty
}

void resumeMock(void *handle)
{
    // empty
}

SystemApi systemApiMock()
{
    return {
        .take = takeMock,
        .give = giveMock,
        .log = logMock,
        .time = timeMock,
        .delay = delayMock,
        .suspend = suspendMock,
        .resume = resumeMock};
}
