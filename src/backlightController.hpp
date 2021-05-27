#include <Arduino.h>

typedef void (*SetBrightnessFunc)(uint8_t level);

typedef struct
{
    SemaphoreHandle_t *backlightLevelMutex;
    uint8_t *backlightLevel;
    SetBrightnessFunc setBrightness;
} BackligthControllerParameters;

void backlightController(BackligthControllerParameters *p);