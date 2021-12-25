#include "sound.hpp"
#include "system.hpp"

static void *queue;
static SystemApi *systemApi; 

void soundBeep()
{
    unsigned char item = 1;
    systemApi->queueSend(queue, &item, 0xffffffffUL); // todo use const for port max delay
}

SoundApi watchSoundApi(void *queueRef, SystemApi *systemApiRef)
{
    queue = queueRef;
    systemApi = systemApiRef;
    return {
        .beep = soundBeep,
    };
}