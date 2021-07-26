#include "system/system.hpp"

#include "watch/watch.hpp"
#include "watch/power.hpp"
#include "watch/rtc.hpp"
#include "supervisor/supervisor.hpp"

#include "task/buttonListener.hpp"
#include "task/showClock.hpp"
#include "task/watchStateProducer.hpp"
#include "task/watchStateRender.hpp"

TTGOClass *watch;

TaskHandle_t buttonListenerTaskHandle;
SemaphoreHandle_t lastShortPressTimestampMutex;
time_t lastShortPressTimestamp;

WatchApi watchApi;
PowerApi powerApi;
SystemApi systemApi;
RtcApi rtcApi;

WatchState watchState;
SemaphoreHandle_t watchStateMutex;

ButtonListenerParameters buttonListenerParameters;
ShowClockParameters showClockParameters;

const unsigned char TASK_COUNT = 1;
TaskParameters *tasks[TASK_COUNT];

SupervisorParameters supervisorParameters;

TaskParameters showClockTaskParameters;
SemaphoreHandle_t showClockTaskTerminationMutex;

WatchStateProducerParameters watchStateProducerParameters;
TaskParameters watchStateProducerTaskParameters;
SemaphoreHandle_t watchStateProducerTerminationMutex;

WatchStateRenderParameters watchStateRenderParameters;
TaskParameters watchStateRenderTaskParameters;
SemaphoreHandle_t watchStateRenderTerminationMutex;

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

void initWatchStateProducerTask() 
{
    watchStateProducerParameters = {
        .stateMutex = &watchStateMutex,
        .state = &watchState,
        .rtcApi = &rtcApi,
        .systemApi = &systemApi
    };

    watchStateProducerTerminationMutex = xSemaphoreCreateMutex();
    
    watchStateProducerTaskParameters = {
        .handle = NULL,
        .func = watchStateProducer,
        .parameters = &watchStateProducerParameters,
        .terminationMutex = &watchStateProducerTerminationMutex,
        .termination = false,
        .canBeSuspended = false,
        .taskDelay = 500,
        .systemApi = &systemApi
    };
}

void initWatchStateRenderTask() 
{
    watchStateRenderParameters = {
        .stateMutex = &watchStateMutex,
        .state = &watchState,
        .systemApi = &systemApi,
        .watch = watch
    };

    watchStateRenderTerminationMutex = xSemaphoreCreateMutex();
    
    watchStateRenderTaskParameters = {
        .handle = NULL,
        .func = watchStateRender,
        .parameters = &watchStateRenderParameters,
        .terminationMutex = &watchStateRenderTerminationMutex,
        .termination = false,
        .canBeSuspended = false,
        .taskDelay = 250,
        .systemApi = &systemApi
    };
}

void setup()
{
    Serial.begin(115200);

    watchApi = defaultWatchApi();
    watchApi.init();
    watchApi.afterWakeUp();

    powerApi = watchPowerApi();
    systemApi = defaultSystemApi();
    rtcApi = watchRtcApi();

    watchState = initialWatchState();
    watchStateMutex = xSemaphoreCreateMutex();
    
    lastShortPressTimestampMutex = xSemaphoreCreateMutex();
    lastShortPressTimestamp = systemApi.time();
    
    buttonListenerParameters = {
        .lastShortPressTimestampMutex = &lastShortPressTimestampMutex,
        .lastShortPressTimestamp = &lastShortPressTimestamp,
        .powerApi = &powerApi,
        .systemApi = &systemApi};

    xTaskCreate(buttonListenerTask, "buttonListenerTask", 2048, (void *)&buttonListenerParameters, 1, &buttonListenerTaskHandle);

    // showClockParameters = {
    //     .watch = watch
    //     };

    // showClockTaskTerminationMutex = xSemaphoreCreateMutex();
    // showClockTaskParameters = {
    //     .handle = NULL,
    //     .func = showClock,
    //     .parameters = &showClockParameters,
    //     .terminationMutex = &showClockTaskTerminationMutex,
    //     .termination = false,
    //     .canBeSuspended = false,
    //     .taskDelay = 250,
    //     .systemApi = &systemApi
    // };
    // xTaskCreate(task, "showClockTask", 2048, (void *)&showClockTaskParameters, 1, &showClockTaskParameters.handle);

    initWatchStateProducerTask();
    xTaskCreate(task, "watchStateProducer", 2048, (void *)&watchStateProducerTaskParameters, 1, &watchStateProducerTaskParameters.handle);

    // initWatchStateRenderTask();
    // xTaskCreate(task, "watchStateRender", 2048, (void *)&watchStateRenderTaskParameters, 1, &watchStateRenderTaskParameters.handle);

    tasks[0] = &watchStateProducerTaskParameters; 
    //tasks[1] = &watchStateRenderTaskParameters;

    supervisorParameters = {
        .lastEventTimestampMutex = &lastShortPressTimestampMutex,
        .lastEventTimestamp = &lastShortPressTimestamp,
        .goToSleepTime = 20,
        .goToSleep = goToSleep,
        .tasks = tasks,
        .tasksCount = TASK_COUNT,
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