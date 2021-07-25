#include "power.hpp"
#include "ttgo.hpp"

// took power from header file name
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

// took watch from package name
PowerApi watchPowerApi()
{
    return {
        .readIRQ = powerReadIRQ,
        .isPEKShortPressIRQ = powerIsPEKShortPressIRQ,
        .clearIRQ = powerClearIRQ};
}