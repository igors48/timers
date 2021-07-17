#include "watch.hpp"
#include <WiFi.h>

void watchInit()
{
    watch = TTGOClass::getWatch();
    watch->begin();
    watch->openBL();
    watch->setBrightness(8);

    watch->rtc->check();
    watch->rtc->syncToSystem();

    WiFi.mode(WIFI_OFF);

    watch->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);

    watch->tft->setCursor(8, 8);
    watch->tft->setTextSize(3);
    watch->tft->setTextFont(2);
    watch->tft->print("ON");
}