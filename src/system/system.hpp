#pragma once

typedef bool (*Take)(void *semaphore, unsigned int blockTime);
typedef bool (*Give)(void *semaphore);
typedef long (*Time)();
typedef void (*Delay)(unsigned int time);
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
    Suspend suspend;
    Resume resume;
} SystemApi;

SystemApi defaultSystemApi();