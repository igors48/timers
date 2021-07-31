#include <stdlib.h>

#include "touchScreenListener.hpp"

#define TOUCH_TRESHOLD 3

void _updateLastUserEventTimestamp(TouchScreenListenerParameters *p) {
    if (p->systemApi->take(p->lastUserEventTimestampMutex, 10))
    {
        long now = p->systemApi->time();
        *p->lastUserEventTimestamp = now;
        p->systemApi->give(p->lastUserEventTimestampMutex);
    }
}

void _touched(TouchScreenListenerParameters *p, signed short x, signed short y)
{
    bool firstTouch = (p->touched == false);
    if (firstTouch)
    {
        p->touched = true;
        p->firstX = x;
        p->firstY = y;
        p->lastX = x;
        p->lastY = y;
    }
    else
    {
        p->lastX = x;
        p->lastY = y;
    }
}

void _notTouched(TouchScreenListenerParameters *p)
{
    bool touchedBefore = (p->touched == true);
    if (touchedBefore)
    {
        p->touched = 0;
        signed short deltaX = abs(p->lastX - p->firstX);
        signed short deltaY = abs(p->lastY - p->firstY);
        bool releasedAroundTheSamePoint = (deltaX < TOUCH_TRESHOLD) && (deltaY < TOUCH_TRESHOLD);
        if (releasedAroundTheSamePoint)
        {
            _updateLastUserEventTimestamp(p);
            p->onTouch(p->firstX, p->firstY);
        }
    }
}

void touchScreenListener(void *v)
{
    TouchScreenListenerParameters *p = (TouchScreenListenerParameters *)v;
    signed short x;
    signed short y;
    bool touched = p->watchApi->getTouch(x, y);
    if (touched)
    {
        _updateLastUserEventTimestamp(p);
        _touched(p, x, y);
    }
    else
    {
        _notTouched(p);
    }
}