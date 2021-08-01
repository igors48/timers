#include "system/system.hpp"

#include "watch/ttgo.hpp"
#include "watch/watch.hpp"
#include "watch/power.hpp"
#include "watch/rtc.hpp"
#include "watch/tft.hpp"
#include "supervisor/supervisor.hpp"

#include "task/buttonListener.hpp"
#include "task/watchStateProducer.hpp"
#include "task/watchStateRender.hpp"
#include "task/touchScreenListener.hpp"

TTGOClass *watch;

SemaphoreHandle_t watchMutex;

// SemaphoreHandle_t lastUserEventTimestampMutex;
time_t lastUserEventTimestamp;

WatchApi watchApi;
PowerApi powerApi;
SystemApi systemApi;
RtcApi rtcApi;
TftApi tftApi;

WatchState watchState;
// SemaphoreHandle_t watchStateMutex;

ButtonListenerParameters buttonListenerParameters;
TaskHandle_t buttonListenerTaskHandle;

const unsigned char TASK_COUNT = 3;
TaskHandle_t tasks[TASK_COUNT];

SupervisorParameters supervisorParameters;

TouchScreenListenerParameters touchScreenListenerParameters;
TaskHandle_t touchScreenListenerTaskHandle;
// TaskParameters touchScreenListenerTaskParameters;
// SemaphoreHandle_t touchScreenListenerTerminationMutex;

WatchStateProducerParameters watchStateProducerParameters;
TaskHandle_t watchStateProducerTaskHandle;
// TaskParameters watchStateProducerTaskParameters;
// SemaphoreHandle_t watchStateProducerTerminationMutex;

WatchStateRenderParameters watchStateRenderParameters;
TaskHandle_t watchStateRenderTaskHandle;
// TaskParameters watchStateRenderTaskParameters;
// SemaphoreHandle_t watchStateRenderTerminationMutex;

// void touchScreenListenerTask(void *v)
// {
//     while (true)
//     {
//         touchScreenListener(v);
//         vTaskDelay(500 / portTICK_PERIOD_MS);
//     }
// }

void buttonListenerTask(void *p)
{
    while (true)
    {
        vTaskSuspend(NULL);
        buttonListener((ButtonListenerParameters *)p);
    }
}

void watchStateProducerTask(void *p)
{
    while (true)
    {
        watchStateProducer(p);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void watchStateRenderTask(void *p)
{
    while (true)
    {
        watchStateRender(p);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void touchScreenListenerTask(void *p)
{
    while (true)
    {
        touchScreenListener(p);
        vTaskDelay(50 / portTICK_PERIOD_MS);
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

void onTouchStub(signed short x, signed short y)
{
    Serial.printf("%d %d \r\n", x, y);
    // temporary - we under critical section, so we can update safely
    watchState.touchX = x;
    watchState.touchY = y;
}

// void initTouchScreenListenerTask()
// {
//     touchScreenListenerParameters = {
//         .touched = false,
//         .firstX = 0,
//         .firstY = 0,
//         .lastX = 0,
//         .lastY = 0,
//         .lastUserEventTimestampMutex = &lastUserEventTimestampMutex,
//         .lastUserEventTimestamp = &lastUserEventTimestamp,
//         .onTouch = onTouchStub,
//         .watchApi = &watchApi,
//         .systemApi = &systemApi};
//     touchScreenListenerTerminationMutex = xSemaphoreCreateMutex();

//     touchScreenListenerTaskParameters = {
//         .handle = NULL,
//         .func = touchScreenListener,
//         .parameters = &touchScreenListenerParameters,
//         .terminationMutex = &touchScreenListenerTerminationMutex,
//         .termination = false,
//         .canBeSuspended = false,
//         .taskDelay = 50,
//         .systemApi = &systemApi};
// }

// void initWatchStateProducerTask()
// {
//     watchStateProducerParameters = {
//         .stateMutex = &watchStateMutex,
//         .state = &watchState,
//         .rtcApi = &rtcApi,
//         .systemApi = &systemApi,
//         .powerApi = &powerApi};

//     watchStateProducerTerminationMutex = xSemaphoreCreateMutex();

//     watchStateProducerTaskParameters = {
//         .handle = NULL,
//         .func = watchStateProducer,
//         .parameters = &watchStateProducerParameters,
//         .terminationMutex = &watchStateProducerTerminationMutex,
//         .termination = false,
//         .canBeSuspended = false,
//         .taskDelay = 500,
//         .systemApi = &systemApi};
// }

// void initWatchStateRenderTask()
// {
//     watchStateRenderParameters = {
//         .stateMutex = &watchStateMutex,
//         .state = &watchState,
//         .systemApi = &systemApi,
//         .tftApi = &tftApi};

//     watchStateRenderTerminationMutex = xSemaphoreCreateMutex();

//     watchStateRenderTaskParameters = {
//         .handle = NULL,
//         .func = watchStateRender,
//         .parameters = &watchStateRenderParameters,
//         .terminationMutex = &watchStateRenderTerminationMutex,
//         .termination = false,
//         .canBeSuspended = false,
//         .taskDelay = 250,
//         .systemApi = &systemApi};
// }

void setup()
{
    Serial.begin(115200);
    delay(4000);

    watchMutex = xSemaphoreCreateMutex();

    if (xSemaphoreTake(watchMutex, 1000 / portTICK_PERIOD_MS))
    {
        watchApi = defaultWatchApi();
        watchApi.init();
        delay(500);
        watchApi.afterWakeUp();
        delay(500);

        powerApi = watchPowerApi();
        systemApi = defaultSystemApi();
        rtcApi = watchRtcApi();
        tftApi = watchTftApi();

        lastUserEventTimestamp = systemApi.time();

        buttonListenerParameters = {
            .lastUserEventTimestampMutex = &watchMutex,
            .lastUserEventTimestamp = &lastUserEventTimestamp,
            .powerApi = &powerApi,
            .systemApi = &systemApi};
        xTaskCreate(buttonListenerTask, "buttonListenerTask", 2048, (void *)&buttonListenerParameters, 1, &buttonListenerTaskHandle);

        pinMode(AXP202_INT, INPUT_PULLUP);
        attachInterrupt(AXP202_INT, buttonInterruptHandler, FALLING);

        watchStateProducerParameters = {
            .stateMutex = &watchMutex,
            .state = &watchState,
            .rtcApi = &rtcApi,
            .systemApi = &systemApi,
            .powerApi = &powerApi,
        };
        xTaskCreate(watchStateProducerTask, "watchStateProducerTask", 2048, (void *)&watchStateProducerParameters, 1, &watchStateProducerTaskHandle);

        watchStateRenderParameters = {
            .stateMutex = &watchMutex,
            .state = &watchState,
            .systemApi = &systemApi,
            .tftApi = &tftApi,
        };
        xTaskCreate(watchStateRenderTask, "watchStateRenderTask", 2048, (void *)&watchStateRenderParameters, 1, &watchStateRenderTaskHandle);

        touchScreenListenerParameters = {
            .touched = false,
            .firstX = 0,
            .firstY = 0,
            .lastX = 0,
            .lastY = 0,
            .lastUserEventTimestampMutex = &watchMutex,
            .lastUserEventTimestamp = &lastUserEventTimestamp,
            .onTouch = onTouchStub,
            .watchApi = &watchApi,
            .systemApi = &systemApi,
        };
        xTaskCreate(touchScreenListenerTask, "touchScreenListenerTask", 2048, (void *)&touchScreenListenerParameters, 1, &touchScreenListenerTaskHandle);

        tasks[0] = watchStateProducerTaskHandle;
        tasks[1] = watchStateRenderTaskHandle;
        tasks[2] = touchScreenListenerTaskHandle;

        supervisorParameters = {
            .lastEventTimestampMutex = &watchMutex,
            .lastEventTimestamp = &lastUserEventTimestamp,
            .goToSleepTime = 5,
            .goToSleep = goToSleep,
            .tasks = tasks,
            .tasksCount = TASK_COUNT,
            .systemApi = &systemApi,
            .watchApi = &watchApi};

        xTaskCreate(supervisorTask, "supervisorTask", 2048, (void *)&supervisorParameters, 1, NULL);

        xSemaphoreGive(watchMutex);
    }
    else
    {
        Serial.println("Could not take watch mutex");
    }

    // watchState = initialWatchState();
    // watchStateMutex = xSemaphoreCreateMutex();

    // lastUserEventTimestampMutex = xSemaphoreCreateMutex();

    //initWatchStateProducerTask();
    //xTaskCreate(task, "watchStateProducer", 2048, (void *)&watchStateProducerTaskParameters, 1, &watchStateProducerTaskParameters.handle);

    //initWatchStateRenderTask();
    //xTaskCreate(task, "watchStateRender", 4096, (void *)&watchStateRenderTaskParameters, 1, &watchStateRenderTaskParameters.handle);

    //initTouchScreenListenerTask();
    //xTaskCreate(task, "touchScreenListener", 16384, (void *)&touchScreenListenerTaskParameters, 1, &touchScreenListenerTaskParameters.handle);
    //xTaskCreate(touchScreenListenerTask, "touchScreenListenerTask", 2048, (void *)&touchScreenListenerParameters, 1, NULL);

    //tasks[0] = &watchStateProducerTaskParameters;
    //tasks[1] = &watchStateRenderTaskParameters;
    //    tasks[2] = &touchScreenListenerTaskParameters;

    // supervisorParameters = {
    //     .lastEventTimestampMutex = &lastUserEventTimestampMutex,
    //     .lastEventTimestamp = &lastUserEventTimestamp,
    //     .goToSleepTime = 5,
    //     .goToSleep = goToSleep,
    //     .tasks = tasks,
    //     .tasksCount = TASK_COUNT,
    //     .systemApi = &systemApi,
    //     .watchApi = &watchApi};

    // xTaskCreate(supervisorTask, "supervisorTask", 2048, (void *)&supervisorParameters, 1, NULL);

    Serial.println("tasks started");
}

void loop()
{
    vTaskSuspend(NULL);
}