#include "bmaMock.hpp"

unsigned int getCounterMock()
{
    return 100500;
}

BmaApi bmaApiMock()
{
    return {
        .getCounter = getCounterMock,
    };
}