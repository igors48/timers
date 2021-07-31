#include "touchScreenListener.hpp"

void _touched(TouchScreenListenerParameters *v, signed short x, signed short y)
{
    bool firstTouch = (v->touched == false);
    if (firstTouch)
    {
        v->touched = true;
        v->firstX = x;
        v->firstY = y;
        v->lastX = x;
        v->lastY = y;
    }
}

void _notTouched(TouchScreenListenerParameters *v)
{

}

void touchScreenListener(void *v)
{
    TouchScreenListenerParameters *p = (TouchScreenListenerParameters *)v;
    signed short x;
    signed short y;
    if (p->watchApi->getTouch(x, y))
    {
        _touched(p, x, y);
    }
    else
    {
        _notTouched(p);
    }
}