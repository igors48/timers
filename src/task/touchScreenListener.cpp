#include <Arduino.h>

#include "touchScreenListener.hpp"

void touchScreenListener(void *v){
    TouchScreenListenerParameters *p = (TouchScreenListenerParameters *)v;
    signed short x;
    signed short y;
    if (p->watchApi->getTouch(x, y))
    {
        Serial.printf("touch %d, %d", x, y);
    }
}