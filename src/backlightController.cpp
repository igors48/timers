#include "backlightController.hpp"

void backlightController(BackligthControllerParameters *p)
{
    p->log("bl started");
    if (p->take(p->backlightLevelMutex, 10) == 1)
    {
        p->log("bl taken");
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