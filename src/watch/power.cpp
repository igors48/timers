#include "power.hpp"
#include "watch.hpp"

void powerReadIRQ()
{
    watch->power->readIRQ();
}

bool powerIsPEKShortPressIRQ()
{
    return watch->power->isPEKShortPressIRQ();
}

void powerClearIRQ()
{
    watch->power->clearIRQ();
}

PowerApi defaultPowerApi()
{
    PowerApi powerApi = {

    };

    return powerApi;
}