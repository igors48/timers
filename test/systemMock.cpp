#include <stdlib.h>

#include "systemMock.hpp"

// todo consider verification of take count and give count

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

void delayUntilMock(unsigned int *prevoiusWakeTimeMillis, unsigned int timeIncrementMillis)
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

void* allocateMock(unsigned int size)
{
    return malloc(size);
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
        .resume = resumeMock,
        .allocate = allocateMock,
        };
}
