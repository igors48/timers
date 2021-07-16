#include "system/system.hpp"

#include "watch/watch.hpp"
#include "watch/power.hpp"
#include "supervisor/supervisor.hpp"

#include "task/buttonListener.hpp"
#include "task/showClock.hpp"

TTGOClass *watch;

SemaphoreHandle_t actionModeMutex = NULL;
bool actionMode = true;

TaskHandle_t buttonListenerTaskHandle = NULL;
SemaphoreHandle_t lastShortPressTimestampMutex = NULL;
time_t lastShortPressTimestamp = 0;

PowerApi powerApi;
SystemApi systemApi;

ButtonListenerParameters buttonListenerParameters;
ShowClockParameters showClockParameters;

TaskParameters *actionModeTasks[10];
TaskParameters *sleepModeTasks[10];
SupervisorParameters supervisorParameters;

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
        vTaskDelay(1000 / portTICK_PERIOD_MS);
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

    actionModeMutex = xSemaphoreCreateMutex();

    supervisorParameters = {
        .actionMutex = actionModeMutex,
        .action = &actionMode,
        .lastEventTimestampMutex = lastShortPressTimestampMutex,
        .lastEventTimestamp = &lastShortPressTimestamp,
        .wakeUpTime = 1,
        .wakeUp = wakeUp,
        .goToSleepTime = 10,
        .goToSleep = goToSleep,
        .actionModeTasks = actionModeTasks,
        .actionModeTasksCount = 0,
        .sleepModeTasks = sleepModeTasks,
        .sleepModeTasksCount = 0,
        .systemApi = &systemApi
    };

    xTaskCreate(supervisorTask, "supervisorTask", 2048, (void *)&supervisorParameters, 1, NULL);

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