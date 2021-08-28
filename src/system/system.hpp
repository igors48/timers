#pragma once

typedef bool (*Take)(void *semaphore, unsigned int blockTimeMillis);
typedef bool (*Give)(void *semaphore);
typedef long (*Time)();
typedef void (*Delay)(unsigned int time);
typedef void (*DelayUntil)(unsigned int *prevoiusWakeTimeMillis, unsigned int timeIncrementMillis);
typedef void (*Resume)(void *handle);
typedef void (*Suspend)(void *handle);
typedef void (*Log)(const char *source, const char *message, ...);

typedef struct
{
    Take take;
    Give give;
    Log log;
    Time time;
    Delay delay;
    DelayUntil delayUntil;
    Suspend suspend;
    Resume resume;
} SystemApi;

SystemApi defaultSystemApi();