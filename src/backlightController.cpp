#include "backlightController.hpp"

void backlightController(BackligthControllerParameters *p)
{
    if (p->take(p->backlightLevelMutex, 10) == 1)
    {
        unsigned char current = *p->backlightLevel;
        p->give(p->backlightLevelMutex);
        //todo - mutex needed here?
        p->setBrightness(current);
        p->log("brightness set to " + current);
    }
    else
    {
        p->log("backlightLevelMutex couldnt obtain from noEventsMonitor");
    }
}