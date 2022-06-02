#pragma once

#include "apps/timer/timeKeeper.hpp"
#include "core/system/sound.hpp"

typedef enum {
    TMR_OK,
    TMR_ERROR,
} TimerResponse ;

typedef enum {
    TMS_IDLE,
    TMS_RUN,
    TMS_ALARM,
} TimerState;

typedef struct
{
    TimeKeeper timeKeeper;
    unsigned int alarmDuration;
    unsigned int lastBeep;
    TimerState state;
    SoundApi *soundApi;
} Timer;

TimerResponse timerStart(Timer *timer, unsigned int duration, unsigned int tickCount);
TimerResponse timerStop(Timer *timer);

void timerTick(Timer *timer, unsigned int tickCount); 

Timer timerCreate(SoundApi *soundApi);


