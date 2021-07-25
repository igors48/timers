#include "powerMock.hpp"

void readIRQMock()
{
    // empty
}

bool isPEKShortPressIRQMock()
{
    return true;
}

void clearIRQMock()
{
    // empty
}

PowerApi powerApiMock()
{
    return {
        .readIRQ = readIRQMock,
        .isPEKShortPressIRQ = isPEKShortPressIRQMock,
        .clearIRQ = clearIRQMock};
}