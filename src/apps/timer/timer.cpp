/**
 * @file 
 * @author Igor Usenko (github: igors48) 
 * @brief timer implementation
*/
#include "timer.hpp"

/**
 * @brief pause between beeps in milliseconds
 */
const unsigned short BEEP_PAUSE = 500;

/**
 * @brief total alarm duration in milliseconds
 */
const unsigned int ALARM_DURATION = 3000;

TimerResponse timerStart(Timer *timer, unsigned long duration, unsigned long tickCount)
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

static void tickRun(Timer *timer, unsigned long tickCount)
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

static void tickAlarm(Timer *timer, unsigned long tickCount)
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

void timerTick(Timer *timer, unsigned long tickCount)
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