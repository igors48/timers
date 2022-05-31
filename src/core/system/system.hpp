#pragma once

typedef void (*Resume)(void *handle); // todo seems not needed anymore
typedef void (*Suspend)(void *handle); // todo seems not needed anymore

typedef struct
{
    bool (*take)(void *semaphore, unsigned int blockTimeMillis);
    bool (*give)(void *semaphore);
    void (*log)(const char *source, const char *message, ...);
    long (*time)();
    void (*delay)(unsigned int time);
    void (*delayUntil)(unsigned int *prevoiusWakeTimeMillis, unsigned int timeIncrementMillis); // todo check usage
    Suspend suspend; // todo seems not needed anymore
    Resume resume; // todo seems not needed anymore
    bool (*queueReceive)(void *queue, void *buffer, unsigned int blockTimeMillis);
    bool (*queueSend)(void *queue, void *item, unsigned int blockTimeMillis);
    unsigned int (*getTickCount)();
    void* (*allocate)(unsigned int size);
} SystemApi;

SystemApi defaultSystemApi();