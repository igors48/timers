#include "soundPlayer.hpp"

static const char SOUND_PLAYER[] = "soundPlayer";

void soundPlayer(SoundPlayerParameters *p)
{
    unsigned char code;
    if ((p->systemApi->queueReceive)(p->queue, &code, 0xffffffffUL)) // todo consider how to use the portMAX_DELAY const here
    {

    }
}