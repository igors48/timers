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
    unsigned short period;
    unsigned short counter;
    unsigned char alarmDuration;
    TimerState state;
} Timer;

TimerResponse setPeriod(Timer *timer, unsigned short seconds);
TimerResponse start(Timer *timer);
TimerResponse stop(Timer *timer);
TimerResponse reset(Timer *timer);
TimerResponse stopAlarm(Timer *timer);

void tick(Timer *timer); 

