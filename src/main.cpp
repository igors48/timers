#include "system/system.hpp"

#include "watch/watch.hpp"
#include "watch/power.hpp"
#include "supervisor/supervisor.hpp"

#include "task/buttonListener.hpp"
#include "task/showClock.hpp"

TTGOClass *watch;

TaskHandle_t buttonListenerTaskHandle;
SemaphoreHandle_t lastShortPressTimestampMutex;
time_t lastShortPressTimestamp;

WatchApi watchApi;
PowerApi powerApi;
SystemApi systemApi;

ButtonListenerParameters buttonListenerParameters;
ShowClockParameters showClockParameters;

TaskParameters *tasks[1];

SupervisorParameters supervisorParameters;

TaskParameters showClockTaskParameters;
SemaphoreHandle_t showClockTaskTerminationMutex;

void buttonListenerTask(void *p)
{
    while (true)
    {
        vTaskSuspend(NULL);
        buttonListener((ButtonListenerParameters *)p);
    }
}

void supervisorTask(void *p)
{
    while (true)
    {
        supervisor((SupervisorParameters *)p);
        vTaskDelay(1000 / portTICK_PERIOD_MS); // todo supervisor task delay -> const
    }
}

void buttonInterruptHandler(void)
{
    vTaskResume(buttonListenerTaskHandle);
}

void setup()
{
    Serial.begin(115200);

    watchApi = defaultWatchApi();
    watchApi.init();
    watchApi.afterWakeUp();

    powerApi = watchPowerApi();
    systemApi = defaultSystemApi();

    lastShortPressTimestampMutex = xSemaphoreCreateMutex();
    lastShortPressTimestamp = systemApi.time();
    
    buttonListenerParameters = {
        .lastShortPressTimestampMutex = &lastShortPressTimestampMutex,
        .lastShortPressTimestamp = &lastShortPressTimestamp,
        .powerApi = &powerApi,
        .systemApi = &systemApi};

    xTaskCreate(buttonListenerTask, "buttonListenerTask", 2048, (void *)&buttonListenerParameters, 1, &buttonListenerTaskHandle);

    showClockParameters = {
        .watch = watch
        };

    showClockTaskTerminationMutex = xSemaphoreCreateMutex();
    showClockTaskParameters = {
        .handle = NULL,
        .func = showClock,
        .parameters = &showClockParameters,
        .terminationMutex = &showClockTaskTerminationMutex,
        .termination = false,
        .canBeSuspended = false,
        .taskDelay = 250,
        .systemApi = &systemApi
    };
    xTaskCreate(task, "showClockTask", 2048, (void *)&showClockTaskParameters, 1, &showClockTaskParameters.handle);

    tasks[0] = &showClockTaskParameters;

    supervisorParameters = {
        .lastEventTimestampMutex = &lastShortPressTimestampMutex,
        .lastEventTimestamp = &lastShortPressTimestamp,
        .goToSleepTime = 5,
        .goToSleep = goToSleep,
        .tasks = tasks,
        .tasksCount = 1,
        .systemApi = &systemApi,
        .watchApi = &watchApi
    };

    xTaskCreate(supervisorTask, "supervisorTask", 2048, (void *)&supervisorParameters, 1, NULL);

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, buttonInterruptHandler, FALLING);

    Serial.println("tasks started");
}

void loop()
{
    vTaskSuspend(NULL);
}