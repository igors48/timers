#include <stdlib.h>

#include "touchScreenListener.hpp"

#include <stdio.h>

static const char TOUCH_SCREEN_LISTENER[] = "touchScreenListener";

void _updateLastUserEventTimestamp(TouchScreenListenerParameters *p)
{
    long now = p->systemApi->time();
    *p->lastUserEventTimestamp = now;
}

void _touched(TouchScreenListenerParameters *p, signed short x, signed short y)
{
    p->lastX = x;
    p->lastY = y;
    bool firstTouch = (p->target == NULL);
    if (firstTouch)
    {
        p->target = p->findTarget(x, y);
        if (p->target != NULL)
        {
            p->target->onTouch(p->target, x, y);
        }
    }
    else
    {
        printf("not first touch\r\n");
    }
    _updateLastUserEventTimestamp(p);
}

void _notTouched(TouchScreenListenerParameters *p)
{
    Component *target = p->target;
    bool touchedBefore = (target != NULL);
    if (touchedBefore)
    {
        target->onRelease(target, p->lastX, p->lastY);
        p->target = NULL;
        _updateLastUserEventTimestamp(p);
    }
}

void touchScreenListener(void *v)
{
    TouchScreenListenerParameters *p = (TouchScreenListenerParameters *)v;
    if (p->systemApi->take(p->watchMutex, 10))
    {
        signed short x;
        signed short y;
        bool touched = p->watchApi->getTouch(x, y);
        if (touched)
        {
            _touched(p, x, y);
        }
        else
        {
            _notTouched(p);
        }
        p->systemApi->give(p->watchMutex);
    }
    else
    {
        p->systemApi->log(TOUCH_SCREEN_LISTENER, "failed to take last event watch mutex");
    }
}