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

int getBattPercentageMock() {
    return 42;
}

PowerApi powerApiMock()
{
    return {
        .readIRQ = readIRQMock,
        .isPEKShortPressIRQ = isPEKShortPressIRQMock,
        .clearIRQ = clearIRQMock,
        .getBattPercentage = getBattPercentageMock};
}