#include "showClock.hpp"

void showClock(void *v)
{
    ShowClockParameters *p = (ShowClockParameters *)v;
    auto tnow = p->watch->rtc->formatDateTime(PCF_TIMEFORMAT_HMS);
    unsigned char battPercent = p->watch->power->getBattPercentage();    
    p->watch->tft->setCursor(25, 90);
    p->watch->tft->setTextSize(4);
    p->watch->tft->setTextFont(1);
    p->watch->tft->print(tnow);
    p->watch->tft->setCursor(100, 130);
    p->watch->tft->setTextSize(2);
    p->watch->tft->print(battPercent);
    p->watch->tft->print(" %");
}