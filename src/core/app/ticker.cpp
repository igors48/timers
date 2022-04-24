#include "ticker.hpp"

void ticker(void *p) {
    TickerParameters *v = (TickerParameters *)p;
    while (true)
    {
        (v->func)();
        // vtaskdelay or system api?
    }
    
}