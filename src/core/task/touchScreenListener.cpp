#include <stddef.h>
#include <stdlib.h>

#include <Arduino.h>

#include "touchScreenListener.hpp"

static const char TOUCH_SCREEN_LISTENER[] = "touchScreenListener";
static unsigned char GESTURE_TRESHOLD = 120;

static void updateLastUserEventTimestamp(TouchScreenListenerParameters *p)
{
    const long now = (p->systemApi->time)();
    *p->lastUserEventTimestamp = now;
}

static short gestureLike(short firstX, short firstY, short lastX, short lastY)
{
    const short dX = lastX - firstX;
    const short dY = lastY - firstY;
    const unsigned short absDx = abs(dX);
    const unsigned short absDy = abs(dY);
    const unsigned short maxD = absDx > absDy ? absDx : absDy;
    if (maxD > GESTURE_TRESHOLD)
    {
        return maxD - GESTURE_TRESHOLD;
    }
    else
    {
        return -1;
    }
}

static void fadeBackLight(TouchScreenListenerParameters *p, short diff)
{
    if (diff < 0)
    {
        p->watchApi->setBrightness(255);
        return;
    }
    if (diff < 25)
    {
        p->watchApi->setBrightness(128);
        return;
    }
    if (diff < 50)
    {
        p->watchApi->setBrightness(96);
        return;
    }
    if (diff < 75)
    {
        p->watchApi->setBrightness(64);
        return;
    }
    if (diff < 100)
    {
        p->watchApi->setBrightness(32);
        return;
    }
    p->watchApi->setBrightness(16);
}

static void touched(TouchScreenListenerParameters *p, signed short x, signed short y)
{
    p->lastX = x;
    p->lastY = y;
    const bool firstTouch = (p->firstX == -1) && (p->firstY == -1);
    Tiler *tiler = p->tiler;
    if (firstTouch)
    {
        p->firstX = x;
        p->firstY = y;
        p->target = (tiler->find)(x, y);
        if (p->target != NULL)
        {
            const unsigned long tickCount = (p->systemApi->getTickCount)();
            (p->tiler->onTouch)(p->target, x, y, tickCount);
        }
    }
    else
    {
        const short diff = gestureLike(p->firstX, p->firstY, p->lastX, p->lastY);
        fadeBackLight(p, diff);
        if (p->target != NULL) 
        {
            const unsigned long tickCount = (p->systemApi->getTickCount)();
            (p->tiler->onMove)(p->target, x, y, tickCount);
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
    const signed short dX = lastX - firstX;
    const signed short dY = lastY - firstY;
    const unsigned short absDx = abs(dX);
    const unsigned short absDy = abs(dY);
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
    const bool touchedBefore = (p->firstX != -1) && (p->firstY != -1);;
    if (touchedBefore)
    {
        Component *target = p->target;
        if (target != NULL)
        {
            const unsigned long tickCount = (p->systemApi->getTickCount)();
            (p->tiler->onRelease)(target, p->lastX, p->lastY, tickCount);
            p->target = NULL;
        }
        const Gesture gesture = detectGesture(p->firstX, p->firstY, p->lastX, p->lastY);
        if (gesture != NONE)
        {
            (p->tiler->onGesture)(gesture);
        }
        p->firstX = -1;
        p->firstY = -1;
        p->watchApi->setBrightness(255);
        updateLastUserEventTimestamp(p);
    }
}

void touchScreenListener(void *v)
{
    TouchScreenListenerParameters *p = (TouchScreenListenerParameters *)v;
    if ((p->systemApi->take)(p->watchMutex, 50))
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