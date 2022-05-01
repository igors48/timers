#include "core/system/system.hpp"
#include "core/system/sound.hpp"

#include "core/watch/ttgo.hpp"
#include "core/watch/watch.hpp"
#include "core/watch/power.hpp"
#include "core/watch/rtc.hpp"
#include "core/watch/tft.hpp"
#include "core/watch/bma.hpp"
#include "core/watch/motor.hpp"
#include "core/watch/i2s.hpp"

#include "core/supervisor/supervisor.hpp"

#include "core/task/buttonListener.hpp"
#include "core/task/touchScreenListener.hpp"
#include "core/task/serviceProcedure.hpp"
#include "core/task/soundPlayer.hpp"

#include "core/app/ticker.hpp"
#include "core/app/manager.hpp"

#include "apps/clock/clockApp.hpp"
#include "apps/step/stepApp.hpp"

TTGOClass *watch;

SemaphoreHandle_t watchMutex;

SystemApi systemApi;
SoundApi soundApi;

WatchApi watchApi;
PowerApi powerApi;
RtcApi rtcApi;
TftApi tftApi;
BmaApi bmaApi;
MotorApi motorApi;
I2sApi i2sApi;

time_t lastUserEventTimestamp; // todo consider rename. this value is updated by timer wakeup also

ButtonListenerParameters buttonListenerParameters;
TaskHandle_t buttonListenerTaskHandle;

const unsigned char TASK_COUNT = 1;
TaskHandle_t tasks[TASK_COUNT];

SupervisorParameters supervisorParameters;

TouchScreenListenerParameters touchScreenListenerParameters;
TaskHandle_t touchScreenListenerTaskHandle;

ServiceProcedureParameters serviceProcedureParameters;

Tiler tiler;

QueueHandle_t soundQueue;
SoundPlayerParameters soundPlayerParameters;

TickerParameters clockAppTickerParameters;
TaskHandle_t clockAppTickerTaskHandle;
App clockApp;

TickerParameters stepAppTickerParameters;
TaskHandle_t stepAppTickerTaskHandle;
App stepApp;

const unsigned char APPS_COUNT = 2;
void *apps[APPS_COUNT];

Manager manager;

void buttonListenerTask(void *p)
{
    while (true)
    {
        vTaskSuspend(NULL);
        buttonListener((ButtonListenerParameters *)p);
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

void soundPlayerTask(void *p)
{
    while (true)
    {
        soundPlayer((SoundPlayerParameters *)p);
    }
}

void buttonInterruptHandler(void)
{
    vTaskResume(buttonListenerTaskHandle);
}

void createClockApplication()
{
    clockAppTickerParameters = {
        .watchMutex = &watchMutex,
        .delayMs = 250,
        .func = clockAppTick,
        .systemApi = &systemApi,
    };
    xTaskCreate(tickerTask, "clockAppTickerTask", 2048, (void *)&clockAppTickerParameters, 1, &clockAppTickerTaskHandle);
    vTaskSuspend(clockAppTickerTaskHandle);

    clockApp = createClockApp(clockAppTickerTaskHandle, &systemApi, &rtcApi, &powerApi, &bmaApi, &tiler, &manager);
}

void createStepApplication()
{
    stepAppTickerParameters = {
        .watchMutex = &watchMutex,
        .delayMs = 500,
        .func = stepAppTick,
        .systemApi = &systemApi,
    };
    xTaskCreate(tickerTask, "stepAppTickerTask", 2048, (void *)&stepAppTickerParameters, 1, &stepAppTickerTaskHandle);
    vTaskSuspend(stepAppTickerTaskHandle);

    stepApp = createStepApp(stepAppTickerTaskHandle, &systemApi, &bmaApi, &tiler, &manager);
}

void setup()
{
    Serial.begin(115200);
    delay(4000);

    watchMutex = xSemaphoreCreateMutex();
    soundQueue = xQueueCreate(100, sizeof(char));

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
        i2sApi = watchI2sApi();

        soundApi = watchSoundApi(&soundQueue, &systemApi);

        lastUserEventTimestamp = systemApi.time();

        buttonListenerParameters = {
            .watchMutex = &watchMutex,
            .lastUserEventTimestamp = &lastUserEventTimestamp,
            .powerApi = &powerApi,
            .systemApi = &systemApi};
        xTaskCreate(buttonListenerTask, "buttonListenerTask", 2048, (void *)&buttonListenerParameters, 1, &buttonListenerTaskHandle);

        pinMode(AXP202_INT, INPUT_PULLUP);
        attachInterrupt(AXP202_INT, buttonInterruptHandler, FALLING);

        tiler = createTiler(&tftApi);

        touchScreenListenerParameters = {
            .target = NULL,
            .firstX = -1,
            .firstY = -1,
            .lastX = 0,
            .lastY = 0,
            .watchMutex = &watchMutex,
            .lastUserEventTimestamp = &lastUserEventTimestamp,
            .tiler = &tiler,
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

        soundPlayerParameters = {
            .queue = &soundQueue,
            .i2sApi = &i2sApi,
            .systemApi = &systemApi,
        };
        xTaskCreate(soundPlayerTask, "soundPlayerTask", 2048, (void *)&soundPlayerParameters, 1, NULL);

        tasks[0] = touchScreenListenerTaskHandle;

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

        createClockApplication();
        createStepApplication();
        
        apps[0] = &clockApp;
        apps[1] = &stepApp;

        manager = createManager(APPS_COUNT, apps, &tiler);
        manager.activateApp(0);

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