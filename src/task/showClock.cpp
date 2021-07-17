#include "showClock.hpp"

void showClock(void *v)
{
    ShowClockParameters *p = (ShowClockParameters *)v;
    auto tnow = p->watch->rtc->formatDateTime(PCF_TIMEFORMAT_HMS);
    p->watch->tft->setCursor(8, 8);
    p->watch->tft->setTextSize(3);
    p->watch->tft->setTextFont(2);
    p->watch->tft->print(tnow);
}