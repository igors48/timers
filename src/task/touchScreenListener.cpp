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
    else
    {
        v->lastX = x;
        v->lastY = y;
    }
}

void _notTouched(TouchScreenListenerParameters *v)
{
    bool touchedBefore = (v->touched == true);
    if (touchedBefore)
    {
        v->touched = 0;
        signed short deltaX = abs(v->lastX - v->firstX);
        signed short deltaY = abs(v->lastY - v->firstY);
        bool releasedAroundTheSamePoint = (deltaX < 3) && (deltaY < 3);
        if (releasedAroundTheSamePoint)
        {
            v->onTouch(v->firstX, v->firstY);
        }
    }
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