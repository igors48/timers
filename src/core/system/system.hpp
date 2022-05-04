#pragma once

typedef void (*Resume)(void *handle);
typedef void (*Suspend)(void *handle);

typedef struct
{
    bool (*take)(void *semaphore, unsigned int blockTimeMillis);
    bool (*give)(void *semaphore);
    void (*log)(const char *source, const char *message, ...);
    long (*time)();
    void (*delay)(unsigned int time);
    void (*delayUntil)(unsigned int *prevoiusWakeTimeMillis, unsigned int timeIncrementMillis);
    Suspend suspend;
    Resume resume;
    bool (*queueReceive)(void *queue, void *buffer, unsigned int blockTimeMillis);
    bool (*queueSend)(void *queue, void *item, unsigned int blockTimeMillis);
    unsigned int (*getTickCount)();
} SystemApi;

SystemApi defaultSystemApi();