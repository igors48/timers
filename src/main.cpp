#include "system/system.hpp"

#include "watch/watch.hpp"
#include "watch/power.hpp"
#include "supervisor/supervisor.hpp"

#include "task/buttonListener.hpp"
#include "task/showClock.hpp"

TTGOClass *watch;

SemaphoreHandle_t actionModeMutex;
bool actionMode;

TaskHandle_t buttonListenerTaskHandle;
SemaphoreHandle_t lastShortPressTimestampMutex;
time_t lastShortPressTimestamp;

PowerApi powerApi;
SystemApi systemApi;

ButtonListenerParameters buttonListenerParameters;
ShowClockParameters showClockParameters;

TaskParameters *actionModeTasks[1];
TaskParameters *sleepModeTasks[0];

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

    watchInit();

    powerApi = watchPowerApi();
    systemApi = defaultSystemApi();

    actionMode = true;

    lastShortPressTimestampMutex = xSemaphoreCreateMutex();
    lastShortPressTimestamp = systemApi.time();
    
    buttonListenerParameters = {
        .lastShortPressTimestampMutex = &lastShortPressTimestampMutex,
        .lastShortPressTimestamp = &lastShortPressTimestamp,
        .powerApi = &powerApi,
        .systemApi = &systemApi};

    xTaskCreate(buttonListenerTask, "buttonListenerTask", 2048, (void *)&buttonListenerParameters, 1, &buttonListenerTaskHandle);

    showClockParameters = {.watch = watch};

    actionModeMutex = xSemaphoreCreateMutex();

    showClockTaskTerminationMutex = xSemaphoreCreateMutex();
    showClockTaskParameters = {
        .handle = NULL,
        .func = showClock,
        .parameters = &showClockParameters,
        .actionMutex = &actionModeMutex,
        .action = &actionMode,
        .terminationMutex = &showClockTaskTerminationMutex,
        .termination = false,
        .canBeSuspended = false,
        .taskDelay = 250,
        .systemApi = &systemApi
    };
    xTaskCreate(actionModeTask, "showClockTask", 2048, (void *)&showClockTaskParameters, 1, &showClockTaskParameters.handle);

    actionModeTasks[0] = &showClockTaskParameters;

    supervisorParameters = {
        .actionMutex = &actionModeMutex,
        .action = &actionMode,
        .lastEventTimestampMutex = &lastShortPressTimestampMutex,
        .lastEventTimestamp = &lastShortPressTimestamp,
        .wakeUpTime = 1,
        .wakeUp = wakeUp,
        .goToSleepTime = 10,
        .goToSleep = goToSleep,
        .actionModeTasks = actionModeTasks,
        .actionModeTasksCount = 1,
        .sleepModeTasks = sleepModeTasks,
        .sleepModeTasksCount = 0,
        .systemApi = &systemApi,
        .watchGoToSleep = watchGoToSleep,
        .watchWakeUp = watchWakeUp
    };

    xTaskCreate(supervisorTask, "supervisorTask", 2048, (void *)&supervisorParameters, 1, NULL);

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, buttonInterruptHandler, FALLING);
    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    watch->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    watch->power->clearIRQ();

    Serial.println("tasks started");
}

void loop()
{
    vTaskSuspend(NULL);
}