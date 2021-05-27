#include "backlightController.hpp"

void backlightController(BackligthControllerParameters *p)
{
    if (xSemaphoreTake(*p->backlightLevelMutex, (TickType_t)10) == pdTRUE)
    {
        uint8_t current = *p->backlightLevel;
        xSemaphoreGive(*p->backlightLevelMutex);
        //todo - mutex needed here?
        p->setBrightness(current);
        Serial.printf("brightness set to %d \r\n", current);
    }
    else
    {
        Serial.println("backlightLevelMutex couldnt obtain from noEventsMonitor");
    }
}