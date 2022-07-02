#pragma once

typedef struct
{
    unsigned long start;
    unsigned long duration;
    void *context;
    void (*mapper)(void *context, float progress);
} Effect;

Effect effectCreate(unsigned long duration, void *context, void (*mapper)(void *context, float progress));
void effectStart(Effect *effect, unsigned long tick);
void effectTick(Effect *effect, unsigned long tick);