#include "bmaMock.hpp"

unsigned int getCounterMock()
{
    return 100500;
}

bool resetStepCounterMock()
{
    return true;
}

BmaApi bmaApiMock()
{
    return {
        .getCounter = getCounterMock,
        .resetStepCounter = resetStepCounterMock,
    };
}