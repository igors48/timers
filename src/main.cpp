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

#include "component/component.hpp"
#include "component/timeDisplayComponent.hpp"
#include "component/batteryDisplayComponent.hpp"
#include "component/touchDisplayComponent.hpp"

TTGOClass *watch;

SemaphoreHandle_t watchMutex;

WatchApi watchApi;
PowerApi powerApi;
SystemApi systemApi;
RtcApi rtcApi;
TftApi tftApi;

time_t lastUserEventTimestamp;
WatchState watchState;

ButtonListenerParameters buttonListenerParameters;
TaskHandle_t buttonListenerTaskHandle;

const unsigned char TASK_COUNT = 3;
TaskHandle_t tasks[TASK_COUNT];

const unsigned char COMPONENTS_COUNT = 4;
Component components[COMPONENTS_COUNT];

SupervisorParameters supervisorParameters;

TouchScreenListenerParameters touchScreenListenerParameters;
TaskHandle_t touchScreenListenerTaskHandle;

WatchStateProducerParameters watchStateProducerParameters;
TaskHandle_t watchStateProducerTaskHandle;

WatchStateRenderParameters watchStateRenderParameters;
TaskHandle_t watchStateRenderTaskHandle;

TimeDisplayComponentState timeDisplayComponentStateOne;
TimeDisplayComponentState timeDisplayComponentStateTwo;

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
        vTaskDelay(300 / portTICK_PERIOD_MS);
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

void onScreenTouchStub(signed short x, signed short y)
{
    Serial.printf("%d %d \r\n", x, y);
    // called from watchMutex critical section, so we can update safely
    watchState.touchX = x;
    watchState.touchY = y;
    for (int i = 0; i < COMPONENTS_COUNT; i++)
    {
        Component current = components[i];
        if ((x > current.x) && (x < current.x + current.w) && (y > current.y) && (y < current.y + current.h))
        {
            Serial.println("bingo");
            current.onTouch(current);
        }
    }
}

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
            .watchMutex = &watchMutex,
            .lastUserEventTimestamp = &lastUserEventTimestamp,
            .powerApi = &powerApi,
            .systemApi = &systemApi};
        xTaskCreate(buttonListenerTask, "buttonListenerTask", 2048, (void *)&buttonListenerParameters, 1, &buttonListenerTaskHandle);

        pinMode(AXP202_INT, INPUT_PULLUP);
        attachInterrupt(AXP202_INT, buttonInterruptHandler, FALLING);

        watchStateProducerParameters = {
            .watchMutex = &watchMutex,
            .state = &watchState,
            .rtcApi = &rtcApi,
            .systemApi = &systemApi,
            .powerApi = &powerApi,
        };
        xTaskCreate(watchStateProducerTask, "watchStateProducerTask", 2048, (void *)&watchStateProducerParameters, 1, &watchStateProducerTaskHandle);

        timeDisplayComponentStateOne = {
            .color = 16,
        };

        timeDisplayComponentStateTwo = {
            .color = 16,
        };

        components[0] = createTimeDisplayComponent(10, 5, 230, 70, &timeDisplayComponentStateOne);
        components[1] = createTimeDisplayComponent(10, 90, 230, 70, &timeDisplayComponentStateTwo);
        components[2] = createBatteryDisplayComponent(100, 150, 50, 50);
        components[3] = createTouchDisplayComponent(0, 200, 200, 50);

        watchStateRenderParameters = {
            .watchMutex = &watchMutex,
            .state = &watchState,
            .systemApi = &systemApi,
            .tftApi = &tftApi,
            .components = components,
            .componentsCount = COMPONENTS_COUNT,
        };
        xTaskCreate(watchStateRenderTask, "watchStateRenderTask", 2048, (void *)&watchStateRenderParameters, 1, &watchStateRenderTaskHandle);

        touchScreenListenerParameters = {
            .touched = false,
            .firstX = 0,
            .firstY = 0,
            .lastX = 0,
            .lastY = 0,
            .watchMutex = &watchMutex,
            .lastUserEventTimestamp = &lastUserEventTimestamp,
            .onScreenTouch = onScreenTouchStub,
            .watchApi = &watchApi,
            .systemApi = &systemApi,
        };
        xTaskCreate(touchScreenListenerTask, "touchScreenListenerTask", 2048, (void *)&touchScreenListenerParameters, 1, &touchScreenListenerTaskHandle);

        tasks[0] = watchStateProducerTaskHandle;
        tasks[1] = watchStateRenderTaskHandle;
        tasks[2] = touchScreenListenerTaskHandle;

        supervisorParameters = {
            .watchMutex = &watchMutex,
            .lastEventTimestamp = &lastUserEventTimestamp,
            .goToSleepTime = 5,
            .goToSleep = goToSleep,
            .tasks = tasks,
            .tasksCount = TASK_COUNT,
            .systemApi = &systemApi,
            .watchApi = &watchApi};

        xTaskCreate(supervisorTask, "supervisorTask", 2048, (void *)&supervisorParameters, 1, NULL);

        xSemaphoreGive(watchMutex);

        Serial.println("tasks started");
    }
    else
    {
        Serial.println("Could not take watch mutex");
    }
}

void loop()
{
    vTaskSuspend(NULL);
}