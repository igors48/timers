#include "soundPlayer.hpp"

static const char SOUND_PLAYER[] = "soundPlayer";

static const unsigned short waveLength = 10;
static const unsigned short volume = 2048;
static const unsigned short samplesCount = 2000; // todo 1/4 sec -> 8000 / 4. it depends on sample rate defined not here. need to refactor
static unsigned int buffer[samplesCount];

static void beep(SoundPlayerParameters *p)
{
    signed short outputValue = volume;
    unsigned short timeAtPeakOrTrough = waveLength;
    for (unsigned short i = 0; i < samplesCount; i++)
    {
        if (timeAtPeakOrTrough == 0)
        {
            if (outputValue == volume)
            {
                outputValue = -volume;
            }
            else
            {
                outputValue = volume;
            }
            timeAtPeakOrTrough = waveLength;
        }
        timeAtPeakOrTrough--;
        unsigned int output = (outputValue << 16) | (outputValue & 0xffff);
        buffer[i] = output;
    }
    (p->i2sApi->write)();
}

void soundPlayer(SoundPlayerParameters *p)
{
    unsigned char code;
    if ((p->systemApi->queueReceive)(p->queue, &code, 0xffffffffUL)) // todo consider how to use the portMAX_DELAY const here
    {
        beep(p);
    }
}