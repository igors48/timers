#pragma once

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
    unsigned int duration; 
    TimeKeeper durationKeeper;
    unsigned char alarmDuration;
    TimeKeeper alarmKeeper;
    TimerState state;
} Timer;

TimerResponse setDuration(Timer *timer, unsigned int millis);
TimerResponse start(Timer *timer);
TimerResponse stop(Timer *timer);
TimerResponse reset(Timer *timer);
TimerResponse stopAlarm(Timer *timer);

void tick(Timer *timer, unsigned int tickCount); 


