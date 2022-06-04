#include "timer.hpp"

const unsigned short BEEP_PAUSE = 500;
const unsigned int ALARM_DURATION = 3000;

TimerResponse timerStart(Timer *timer, unsigned int duration, unsigned int tickCount)
{
    if (timer->state == TMS_ALARM || timer->state == TMS_RUN)
    {
        return TMR_ERROR;
    }
    timer->state = TMS_RUN;
    timeKeeperReset(&(timer->timeKeeper), duration, tickCount);
    return TMR_OK;
}

TimerResponse timerStop(Timer *timer)
{
    timer->state = TMS_IDLE;
    return TMR_OK;
}

static void tickRun(Timer *timer, unsigned int tickCount)
{
    timeKeeperTick(&(timer->timeKeeper), tickCount);
    if (timer->timeKeeper.duration == 0)
    {
        timeKeeperReset(&(timer->timeKeeper), timer->alarmDuration, tickCount);
        timer->lastBeep = tickCount;
        timer->state = TMS_ALARM;
        (timer->soundApi->beep)();
    }
}

static void tickAlarm(Timer *timer, unsigned int tickCount)
{
    timeKeeperTick(&(timer->timeKeeper), tickCount);
    if (timer->timeKeeper.duration == 0)
    {
        timer->state = TMS_IDLE;
        return;
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
        .timeKeeper = timeKeeperCreate(),
        .alarmDuration = ALARM_DURATION,
        .lastBeep = 0,
        .state = TMS_IDLE,
        .soundApi = soundApi,
    };
}