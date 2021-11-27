#include <stddef.h>
#include <stdlib.h>

#include "touchScreenListener.hpp"

static const char TOUCH_SCREEN_LISTENER[] = "touchScreenListener";
static unsigned char GESTURE_TRESHOLD = 120;

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
        if (p->target != NULL) 
        {
            (p->target->onMove)(p->target, x, y);
        }
    }
    updateLastUserEventTimestamp(p);
}

static Gesture detectHorizontalGesture(signed short dX)
{
    if (dX > 0)
    {
        return MOVE_RIGHT;
    }
    else
    {
        return MOVE_LEFT;
    }        
}

static Gesture detectVerticalGesture(signed short dY)
{
    if (dY > 0)
    {
        return MOVE_DOWN;
    }
    else
    {
        return MOVE_UP;
    }        
}

static Gesture detectGesture(signed short firstX, signed short firstY, signed short lastX, signed short lastY)
{
    signed short dX = lastX - firstX;
    signed short dY = lastY - firstY;
    unsigned short absDx = abs(dX);
    unsigned short absDy = abs(dY);
    if ((absDx < GESTURE_TRESHOLD) && (absDy < GESTURE_TRESHOLD))
    {
        return NONE;
    }
    if (absDx > absDy)
    {
        return detectHorizontalGesture(dX);
    }
    else
    {
        return detectVerticalGesture(dY);
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
        Gesture gesture = detectGesture(p->firstX, p->firstY, p->lastX, p->lastY);
        if (gesture != NONE)
        {
            (p->screen->onGesture)(p->screen, gesture);
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