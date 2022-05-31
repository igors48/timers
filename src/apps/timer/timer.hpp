#pragma once

#include "apps/timer/timeKeeper.hpp"
#include "core/system/sound.hpp"

typedef enum {
    TMR_OK,
    TMR_ERROR,
} TimerResponse ;

typedef enum {
    TMS_CREATED,
    TMS_IDLE,
    TMS_RUN,
    TMS_ALARM,
} TimerState;

typedef struct
{
    unsigned int duration; 
    TimeKeeper durationKeeper;
    unsigned char alarmDuration;
    TimeKeeper alarmKeeper;
    unsigned int lastBeep;
    TimerState state;
    SoundApi *soundApi;
} Timer;

TimerResponse timerSetDuration(Timer *timer, unsigned int millis);
TimerResponse timerStart(Timer *timer);
TimerResponse timerStop(Timer *timer);
TimerResponse timerResume(Timer *timer);
TimerResponse timerStopAlarm(Timer *timer);

void timerTick(Timer *timer, unsigned int tickCount); 

Timer timerCreate(SoundApi *soundApi);


