#include "sound.hpp"

static void *queue;
static SystemApi *systemApi; 

static void soundBeep()
{
    unsigned char item = 1;
    (systemApi->queueSend)(queue, &item, 0xffffffffUL); // todo use const for port max delay
    (systemApi->log)("sound api", "beep");
}

SoundApi watchSoundApi(void *queueRef, SystemApi *systemApiRef)
{
    queue = queueRef;
    systemApi = systemApiRef;
    return {
        .beep = soundBeep,
    };
}