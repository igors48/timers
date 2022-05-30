#include "timer.hpp"

TimerResponse setPeriod(Timer *timer, unsigned int millis)
{
    if (timer->state != TMS_IDLE)
    {
        return TMR_ERROR;
    }
    timer->period = millis;
    return TMR_OK;
}

TimerResponse start(Timer *timer)
{
    if (timer->state == TMS_ALARM)
    {
        return TMR_ERROR;
    }
    timer->state = TMS_RUN;
    return TMR_OK;
}

TimerResponse stop(Timer *timer)
{
    if (timer->state == TMS_ALARM)
    {
        return TMR_ERROR;
    }
    timer->state = TMS_IDLE;
    return TMR_OK;
}

TimerResponse reset(Timer *timer)
{
    if (timer->state != TMS_IDLE)
    {
        return TMR_ERROR;
    }
    timer->counter = timer->period;
    return TMR_OK;
}

TimerResponse stopAlarm(Timer *timer)
{
    return TMR_OK;
}

static void tick(TimeKeeper *timeKeeper, unsigned int tickCount)
{
    unsigned int passed = tickCount - timeKeeper->lastTick;
    if (passed >= timeKeeper->counter)
    {
        timeKeeper->counter = 0;
    }
    else
    {
        timeKeeper->counter -= passed;  
    }
    timeKeeper->lastTick = tickCount;
}

void reset(TimeKeeper *timeKeeper, unsigned int counter, unsigned int tickCount)
{

}

static void tickRun(Timer *timer, unsigned int tickCount)
{
    
}

static void tickAlarm(Timer *timer, unsigned int tickCount)
{
    
}

void tick(Timer *timer, unsigned int tickCount)
{
    switch (timer->state)
    {
    case TMS_RUN:
        tickRun(timer, tickCount);
        return;
    case TMS_ALARM:
        tickAlarm(timer, tickCount);
        return;    
    default:
        return;
    }
}
