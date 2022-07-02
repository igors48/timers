#include "effect.hpp"

Effect effectCreate(unsigned long duration, void *context, void (*mapper)(void *context, float progress))
{
    return
    {
        .start = 0,
        .duration = duration,
        .context = context,
        .mapper = mapper,
    };
}

void effectStart(Effect *effect, unsigned long tick)
{
    effect->start = tick;
    (effect->mapper)(effect->context, 0);
}

void effectTick(Effect *effect, unsigned long tick)
{
    const float start = effect->start;
    const float duration = effect->duration;
    float progress = ((float)tick - start) / duration;
    if (progress > 1)
    {
        progress = 1;
    }
    (effect->mapper)(effect->context, progress);
}