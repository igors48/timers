#include "showClock.hpp"

void showClock(void *v)
{
    ShowClockParameters *p = (ShowClockParameters *)v;
    auto tnow = p->watch->rtc->formatDateTime(PCF_TIMEFORMAT_HMS);
    p->watch->tft->setCursor(25, 90);
    p->watch->tft->setTextSize(4);
    p->watch->tft->setTextFont(1);
    p->watch->tft->print(tnow);
}