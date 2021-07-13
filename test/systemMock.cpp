#include "system/system.hpp"

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

SystemApi systemApiMock()
{
    return
    {
        .take = takeMock,
        .give = giveMock,
        .log = logMock,
        .time = timeMock
    };
}
