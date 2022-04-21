#include "bma.hpp"
#include "ttgo.hpp"

unsigned int bmaGetCounter()
{
    return watch->bma->getCounter();
}

bool bmaResetStepCounter()
{
    return watch->bma->resetStepCounter();
}

BmaApi watchBmaApi()
{
    return {
        .getCounter = bmaGetCounter,
        .resetStepCounter = bmaResetStepCounter,
    };
}