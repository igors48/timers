#include "system/system.hpp"

#include "watch/ttgo.hpp"
#include "watch/watch.hpp"
#include "watch/power.hpp"
#include "watch/rtc.hpp"
#include "watch/tft.hpp"
#include "watch/bma.hpp"
#include "watch/motor.hpp"

#include "supervisor/supervisor.hpp"

#include "task/buttonListener.hpp"
#include "task/watchStateProducer.hpp"
#include "task/watchStateRender.hpp"
#include "task/touchScreenListener.hpp"
#include "task/serviceProcedure.hpp"

#include "component/component.hpp"
#include "component/group.hpp"
#include "screen/screen.hpp"

TTGOClass *watch;

SemaphoreHandle_t watchMutex;

WatchApi watchApi;
PowerApi powerApi;
SystemApi systemApi;
RtcApi rtcApi;
TftApi tftApi;
BmaApi bmaApi;
MotorApi motorApi;

time_t lastUserEventTimestamp; // todo consider rename. this value is updated by timer wakeup also
WatchState watchState;

ButtonListenerParameters buttonListenerParameters;
TaskHandle_t buttonListenerTaskHandle;

const unsigned char TASK_COUNT = 3;
TaskHandle_t tasks[TASK_COUNT];

const unsigned char COMPONENTS_COUNT = 8;
void* components[COMPONENTS_COUNT];
GroupState screenState;
Component screen;

SupervisorParameters supervisorParameters;

TouchScreenListenerParameters touchScreenListenerParameters;
TaskHandle_t touchScreenListenerTaskHandle;

WatchStateProducerParameters watchStateProducerParameters;
TaskHandle_t watchStateProducerTaskHandle;

WatchStateRenderParameters watchStateRenderParameters;
TaskHandle_t watchStateRenderTaskHandle;

ServiceProcedureParameters serviceProcedureParameters;

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

void serviceProcedureTask(void *p)
{
    while (true)
    {
        serviceProcedure((ServiceProcedureParameters *)p);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void buttonInterruptHandler(void)
{
    vTaskResume(buttonListenerTaskHandle);
}

// todo take it from here
Component* findTarget(signed short x, signed short y)
{
    // called from watchMutex critical section, so we can update safely
    Serial.printf("%d %d \r\n", x, y);
    watchState.touchX = x;
    watchState.touchY = y;
    // for (int i = 0; i < COMPONENTS_COUNT; i++)
    // {
    //     Component current = components[i];
    //     if ((x > current.x) && (x < current.x + current.w) && (y > current.y) && (y < current.y + current.h))
    //     {
    //         Serial.println("bingo");
    //         motorApi.buzz(10);
    //         //current.onTouch(current);
    //         return &components[i];
    //     }
    // }
    return NULL;
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
        bmaApi = watchBmaApi();
        motorApi = watchMotorApi();

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
            .bmaApi = &bmaApi,
        };
        xTaskCreate(watchStateProducerTask, "watchStateProducerTask", 2048, (void *)&watchStateProducerParameters, 1, &watchStateProducerTaskHandle);

        createComponents(components);       
        screenState = {
            .childrenCount = COMPONENTS_COUNT,
            .children = components
        };
        screen = createGroupComponent(0, 0, &screenState);

        watchStateRenderParameters = {
            .watchMutex = &watchMutex,
            .state = &watchState,
            .systemApi = &systemApi,
            .tftApi = &tftApi,
            .screen = &screen,
        };
        xTaskCreate(watchStateRenderTask, "watchStateRenderTask", 2048, (void *)&watchStateRenderParameters, 1, &watchStateRenderTaskHandle);

        touchScreenListenerParameters = {
            .target = NULL,
            .lastX = 0,
            .lastY = 0,
            .watchMutex = &watchMutex,
            .lastUserEventTimestamp = &lastUserEventTimestamp,
            .findTarget = findTarget,
            .watchApi = &watchApi,
            .systemApi = &systemApi,
        };
        xTaskCreate(touchScreenListenerTask, "touchScreenListenerTask", 2048, (void *)&touchScreenListenerParameters, 1, &touchScreenListenerTaskHandle);

        serviceProcedureParameters = {
            .watchMutex = &watchMutex,
            .lastBuzzHour = 0,
            .lastStepCounterResetDay = 0,
            .rtcApi = &rtcApi,
            .bmaApi = &bmaApi,
            .systemApi = &systemApi,
            .motorApi = &motorApi,
        };
        xTaskCreate(serviceProcedureTask, "serviceProcedureTask", 2048, (void *)&serviceProcedureParameters, 1, NULL);

        tasks[0] = watchStateProducerTaskHandle;
        tasks[1] = watchStateRenderTaskHandle;
        tasks[2] = touchScreenListenerTaskHandle;

        supervisorParameters = {
            .watchMutex = &watchMutex,
            .lastUserEventTimestamp = &lastUserEventTimestamp,
            .goToSleepTime = 5,
            .supervisorSleep = supervisorSleep,
            .tasks = tasks,
            .tasksCount = TASK_COUNT,
            .systemApi = &systemApi,
            .watchApi = &watchApi,
            .rtcApi = &rtcApi,
            };

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