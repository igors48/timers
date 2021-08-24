#include <stdlib.h>

#include "touchScreenListener.hpp"

void _updateLastUserEventTimestamp(TouchScreenListenerParameters *p)
{
    long now = p->systemApi->time();
    *p->lastUserEventTimestamp = now;
}

void _touched(TouchScreenListenerParameters *p, signed short x, signed short y)
{
    bool firstTouch = (p->target == NULL);
    if (firstTouch)
    {
        p->target = p->findTarget(x, y);
        if (p != NULL) 
        {
            p->target->onTouch(*(p->target));
        }
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
    _updateLastUserEventTimestamp(p);
}

bool _insideComponent(signed short x, signed short y, Component *component)
{
    return false;
}

void _notTouched(TouchScreenListenerParameters *p)
{
    Component *target = p->target;
    bool touchedBefore = (target != NULL);
    if (touchedBefore)
    {
        bool releasedInsideComponent = _insideComponent(p->lastX, p->lastY, p->target);    
        if (releasedInsideComponent)
        {            
            target->onRelease(*target);
        }
        else
        {
            target->onSkip(*target);
        }
        p->target = NULL;
    }
    _updateLastUserEventTimestamp(p); 
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
            _updateLastUserEventTimestamp(p);
            _touched(p, x, y);
        }
        else
        {
            _notTouched(p);
        }
        p->systemApi->give(p->watchMutex);
    }
}