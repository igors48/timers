#include "timer.hpp"

const unsigned short BEEP_PAUSE = 500;
const unsigned int ALARM_DURATION = 15000;

TimerResponse timerSetDuration(Timer *timer, unsigned int millis)
{
    if (timer->state != TMS_IDLE)
    {
        return TMR_ERROR;
    }
    timer->duration = millis;
    return TMR_OK;
}

TimerResponse timerStart(Timer *timer, unsigned int tickCount)
{
    if (timer->state == TMS_ALARM)
    {
        return TMR_ERROR;
    }
    timer->state = TMS_RUN;
    timeKeeperReset(&(timer->durationKeeper), timer->duration, tickCount);
    return TMR_OK;
}

TimerResponse timerStop(Timer *timer)
{
    if (timer->state == TMS_ALARM)
    {
        return TMR_ERROR;
    }
    timer->state = TMS_IDLE;
    return TMR_OK;
}

TimerResponse timerResume(Timer *timer)
{
}

TimerResponse timerStopAlarm(Timer *timer)
{
    return TMR_OK;
}

static void tickRun(Timer *timer, unsigned int tickCount)
{
    timeKeeperTick(&(timer->durationKeeper), tickCount);
    if (timer->durationKeeper.counter == 0)
    {
        timeKeeperReset(&(timer->alarmKeeper), timer->alarmDuration, tickCount);
        timer->lastBeep = tickCount;
        timer->state = TMS_ALARM;
    }
}

static void tickAlarm(Timer *timer, unsigned int tickCount)
{
    timeKeeperTick(&(timer->alarmKeeper), tickCount);
    if (timer->durationKeeper.counter == 0)
    {
        timer->state = TMS_IDLE;
    }
    if (tickCount - timer->lastBeep >= BEEP_PAUSE)
    {
        (timer->soundApi->beep)();
        timer->lastBeep = tickCount;
    }
}

void timerTick(Timer *timer, unsigned int tickCount)
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

Timer timerCreate(SoundApi *soundApi)
{
    return {
        .duration = 0,
        .durationKeeper = timeKeeperCreate(),
        .alarmDuration = ALARM_DURATION,
        .alarmKeeper = timeKeeperCreate(),
        .lastBeep = 0,
        .state = TMS_CREATED,
        .soundApi = soundApi,
    };
}