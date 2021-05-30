#include <cstdio>
#include "backlightController.hpp"

void backlightController(BackligthControllerParameters *p)
{
    if (p->take(p->backlightLevelMutex, 10) == 1)
    {
        unsigned char current = *p->backlightLevel;
        p->give(p->backlightLevelMutex);
        //todo - mutex needed here?
        p->setBrightness(current);
        p->log("backlightController", "brightness set to %d", current);
    }
    else
    {
        p->log("backlightController", "failed to take backlightLevelMutex");
    }
}