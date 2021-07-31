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

TaskHandle_t buttonListenerTaskHandle;
SemaphoreHandle_t lastUserEventTimestampMutex;
time_t lastUserEventTimestamp;

WatchApi watchApi;
PowerApi powerApi;
SystemApi systemApi;
RtcApi rtcApi;
TftApi tftApi;

WatchState watchState;
SemaphoreHandle_t watchStateMutex;

ButtonListenerParameters buttonListenerParameters;

const unsigned char TASK_COUNT = 3;
TaskParameters *tasks[TASK_COUNT];

SupervisorParameters supervisorParameters;

TouchScreenListenerParameters touchScreenListenerParameters;
TaskParameters touchScreenListenerTaskParameters;
SemaphoreHandle_t touchScreenListenerTerminationMutex;

WatchStateProducerParameters watchStateProducerParameters;
TaskParameters watchStateProducerTaskParameters;
SemaphoreHandle_t watchStateProducerTerminationMutex;

WatchStateRenderParameters watchStateRenderParameters;
TaskParameters watchStateRenderTaskParameters;
SemaphoreHandle_t watchStateRenderTerminationMutex;

void touchScreenListenerTask(void *v)
{
    while (true)
    {
        touchScreenListener(v);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

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

void onTouchStub(signed short x, signed short y)
{
    Serial.printf("%d %d \r\n", x, y);
}

void initTouchScreenListenerTask()
{
    touchScreenListenerParameters = {
        .touched = false,
        .firstX = 0,
        .firstY = 0,
        .lastX = 0,
        .lastY = 0,
        .lastUserEventTimestampMutex = &lastUserEventTimestampMutex,
        .lastUserEventTimestamp = &lastUserEventTimestamp,
        .onTouch = onTouchStub,
        .watchApi = &watchApi,
        .systemApi = &systemApi
    };

    touchScreenListenerTerminationMutex = xSemaphoreCreateMutex();

    touchScreenListenerTaskParameters = {
        .handle = NULL,
        .func = touchScreenListener,
        .parameters = &touchScreenListenerParameters,
        .terminationMutex = &touchScreenListenerTerminationMutex,
        .termination = false,
        .canBeSuspended = false,
        .taskDelay = 50,
        .systemApi = &systemApi};
}

void initWatchStateProducerTask()
{
    watchStateProducerParameters = {
        .stateMutex = &watchStateMutex,
        .state = &watchState,
        .rtcApi = &rtcApi,
        .systemApi = &systemApi,
        .powerApi = &powerApi};

    watchStateProducerTerminationMutex = xSemaphoreCreateMutex();

    watchStateProducerTaskParameters = {
        .handle = NULL,
        .func = watchStateProducer,
        .parameters = &watchStateProducerParameters,
        .terminationMutex = &watchStateProducerTerminationMutex,
        .termination = false,
        .canBeSuspended = false,
        .taskDelay = 500,
        .systemApi = &systemApi};
}

void initWatchStateRenderTask()
{
    watchStateRenderParameters = {
        .stateMutex = &watchStateMutex,
        .state = &watchState,
        .systemApi = &systemApi,
        .tftApi = &tftApi};

    watchStateRenderTerminationMutex = xSemaphoreCreateMutex();

    watchStateRenderTaskParameters = {
        .handle = NULL,
        .func = watchStateRender,
        .parameters = &watchStateRenderParameters,
        .terminationMutex = &watchStateRenderTerminationMutex,
        .termination = false,
        .canBeSuspended = false,
        .taskDelay = 250,
        .systemApi = &systemApi};
}

void setup()
{
    Serial.begin(115200);
    delay(4000);

    watchApi = defaultWatchApi();
    watchApi.init();
    delay(500);
    watchApi.afterWakeUp();
    delay(500);

    powerApi = watchPowerApi();
    systemApi = defaultSystemApi();
    rtcApi = watchRtcApi();
    tftApi = watchTftApi();

    watchState = initialWatchState();
    watchStateMutex = xSemaphoreCreateMutex();

    lastUserEventTimestampMutex = xSemaphoreCreateMutex();
    lastUserEventTimestamp = systemApi.time();

    buttonListenerParameters = {
        .lastUserEventTimestampMutex = &lastUserEventTimestampMutex,
        .lastUserEventTimestamp = &lastUserEventTimestamp,
        .powerApi = &powerApi,
        .systemApi = &systemApi};
    xTaskCreate(buttonListenerTask, "buttonListenerTask", 2048, (void *)&buttonListenerParameters, 1, &buttonListenerTaskHandle);

    initWatchStateProducerTask();
    xTaskCreate(task, "watchStateProducer", 2048, (void *)&watchStateProducerTaskParameters, 1, &watchStateProducerTaskParameters.handle);

    initWatchStateRenderTask();
    xTaskCreate(task, "watchStateRender", 4096, (void *)&watchStateRenderTaskParameters, 1, &watchStateRenderTaskParameters.handle);

    initTouchScreenListenerTask();
    xTaskCreate(task, "touchScreenListener", 4096, (void *)&touchScreenListenerTaskParameters, 1, &touchScreenListenerTaskParameters.handle);

    tasks[0] = &watchStateProducerTaskParameters;
    tasks[1] = &watchStateRenderTaskParameters;
    tasks[2] = &touchScreenListenerTaskParameters;

    supervisorParameters = {
        .lastEventTimestampMutex = &lastUserEventTimestampMutex,
        .lastEventTimestamp = &lastUserEventTimestamp,
        .goToSleepTime = 5,
        .goToSleep = goToSleep,
        .tasks = tasks,
        .tasksCount = TASK_COUNT,
        .systemApi = &systemApi,
        .watchApi = &watchApi};

    xTaskCreate(supervisorTask, "supervisorTask", 2048, (void *)&supervisorParameters, 1, NULL);

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, buttonInterruptHandler, FALLING);

    Serial.println("tasks started");
}

void loop()
{
    vTaskSuspend(NULL);
}