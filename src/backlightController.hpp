#include <freertos.hpp>
#include <twatch.hpp>

typedef struct
{
    void *backlightLevelMutex;
    unsigned char *backlightLevel;
    SetBrightnessFunc setBrightness;
    Take take;
    Give give;
    Log log;
} BackligthControllerParameters;

void backlightController(BackligthControllerParameters *p);