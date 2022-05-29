#include "timer.hpp"

TimerResponse setPeriod(Timer *timer, unsigned short seconds)
{
    if (timer->state != TMS_IDLE)
    {
        return TMR_ERROR;
    }
    timer->period = seconds;
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

static void tickRun(Timer *timer)
{
    
}

static void tickAlarm(Timer *timer)
{
    
}

void tick(Timer *timer)
{
    switch (timer->state)
    {
    case TMS_RUN:
        tickRun(timer);
        return;
    case TMS_ALARM:
        tickAlarm(timer);
        return;    
    default:
        return;
    }
}
