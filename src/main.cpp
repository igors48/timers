#include "system/system.hpp"

#include "watch/watch.hpp"
#include "watch/power.hpp"

#include "task/buttonListener.hpp"
#include "task/showClock.hpp"

TTGOClass *watch;

TaskHandle_t buttonListenerTaskHandle = NULL;
SemaphoreHandle_t lastShortPressTimestampMutex = NULL;
time_t lastShortPressTimestamp = 0;

PowerApi powerApi;
SystemApi systemApi;

ButtonListenerParameters buttonListenerParameters;
ShowClockParameters showClockParameters;

void buttonListenerTask(void *p)
{
    while (true)
    {
        vTaskSuspend(NULL);
        buttonListener((ButtonListenerParameters *)p);
    }
}

void buttonInterruptHandler(void)
{
    vTaskResume(buttonListenerTaskHandle);
}

void setup()
{
    Serial.begin(115200);

    watchInit();

    powerApi = watchPowerApi();
    systemApi = defaultSystemApi();

    lastShortPressTimestampMutex = xSemaphoreCreateMutex();

    buttonListenerParameters = {
        .lastShortPressTimestampMutex = &lastShortPressTimestampMutex,
        .lastShortPressTimestamp = &lastShortPressTimestamp,
        .powerApi = &powerApi,
        .systemApi = &systemApi};

    xTaskCreate(buttonListenerTask, "buttonListenerTask", 2048, (void *)&buttonListenerParameters, 1, &buttonListenerTaskHandle);

    showClockParameters = {.watch = watch};

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, buttonInterruptHandler, FALLING);
    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    watch->power->clearIRQ();

    Serial.println("tasks started");
}

void loop()
{
    vTaskSuspend(NULL);
}