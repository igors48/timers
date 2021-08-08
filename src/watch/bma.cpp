#include "bma.hpp"
#include "ttgo.hpp"

unsigned int bmaGetCounter()
{
    return watch->bma->getCounter();
}

BmaApi watchBmaApi()
{
    return {
        .getCounter = bmaGetCounter,
    };
}