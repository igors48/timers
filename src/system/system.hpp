#pragma once

typedef bool (*Take)(void *semaphore, unsigned int blockTime);
bool take(void *semaphore, unsigned int blockTime);

typedef bool (*Give)(void *semaphore);
bool give(void *semaphore);

typedef long (*Time)();
long time();

typedef void (*Delay)(unsigned int time);
void frDelay(unsigned int time);

typedef void (*Resume)(void *handle);
void frResume(void *handle); 

typedef void (*Suspend)(void *handle);
void frSuspend(void *handle); 

typedef void (*Log)(const char *source, const char *message, ...);
void log(const char *source, const char *message, ...);

typedef struct
{
    Take take;
    Give give;
} SystemApi;

SystemApi defaultSystemApi();