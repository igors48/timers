#pragma once

typedef struct Timer_ Timer;

typedef enum TimerResponse {
    TMR_OK,
    TMR_ERROR,
};

typedef enum TimerState {
    TMS_IDLE,
    TMS_RUN,
    TMS_ALARM,
};

typedef struct
{
    unsigned short period;
    unsigned short counter;
    unsigned char alarmDuration;
    TimerState state;
} TimerContext;

struct Timer_
{
    void *context;
    TimerResponse (*setPeriod)(Timer *timer, unsigned short seconds);
    TimerResponse (*start)(Timer *timer);
    TimerResponse (*stop)(Timer *timer);
    TimerResponse (*reset)(Timer *timer);
    TimerResponse (*stopAlarm)(Timer *timer);
    TimerResponse (*tick)(Timer *timer); 
};
