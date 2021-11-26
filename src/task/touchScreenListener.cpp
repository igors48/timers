#include <stddef.h>

#include "touchScreenListener.hpp"

static const char TOUCH_SCREEN_LISTENER[] = "touchScreenListener";

static void updateLastUserEventTimestamp(TouchScreenListenerParameters *p)
{
    long now = (p->systemApi->time)();
    *p->lastUserEventTimestamp = now;
}

static void touched(TouchScreenListenerParameters *p, signed short x, signed short y)
{
    p->lastX = x;
    p->lastY = y;
    bool firstTouch = (p->firstX == -1) && (p->firstY == -1);
    Component *screen = p->screen;
    if (firstTouch)
    {
        p->firstX = x;
        p->firstY = y;
        p->target = (screen->contains)(screen, x, y);
        if (p->target != NULL)
        {
            (p->target->onTouch)(p->target, x, y);
        }
    }
    else
    {
        (p->target->onMove)(p->target, x, y);
    }
    updateLastUserEventTimestamp(p);
}

static bool inHotSpot(signed short x, signed short y)
{
    bool xInHotSpot = (x >= 60) && (x <= 180);
    if (!xInHotSpot)
    {
        return xInHotSpot;
    }
    bool yInHotSpot = (y >= 60) && (y <= 180);
    return yInHotSpot;
}

Gesture detectGesture(signed short firstX, signed short firstY, signed short lastX, signed short lastY)
{
    bool firstInHotSpot = inHotSpot(firstX, firstY);
    if (!firstInHotSpot)
    {
        return NONE;
    }
    bool lastInHotSpot = inHotSpot(lastX, lastY);
    if (!lastInHotSpot)
    {
        return NONE;
    }
    signed short dX = abs(lastX - firstX);
    signed short dY = abs(lastY - firstY);
    if (dX > dY)
    {

    }
    else
    {
        
    }
}

static void notTouched(TouchScreenListenerParameters *p)
{
    bool touchedBefore = (p->firstX != -1) && (p->firstY != -1);;
    if (touchedBefore)
    {
        Component *target = p->target;
        if (target != NULL)
        {
            (target->onRelease)(target, p->lastX, p->lastY);
            p->target = NULL;
        }
        p->firstX = -1;
        p->firstY = -1;
        updateLastUserEventTimestamp(p);
    }
}

void touchScreenListener(void *v)
{
    TouchScreenListenerParameters *p = (TouchScreenListenerParameters *)v;
    if ((p->systemApi->take)(p->watchMutex, 10))
    {
        signed short x;
        signed short y;
        bool screenTouched = (p->watchApi->getTouch)(x, y);
        if (screenTouched)
        {
            touched(p, x, y);
        }
        else
        {
            notTouched(p);
        }
        (p->systemApi->give)(p->watchMutex);
    }
    else
    {
        (p->systemApi->log)(TOUCH_SCREEN_LISTENER, "failed to take watch mutex");
    }
}