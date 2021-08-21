#include "motorMock.hpp"

void buzzMock(int duration)
{
    // empty
}

MotorApi motorApiMock()
{
    return {
        .buzz = buzzMock,
    };
}