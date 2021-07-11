#include "system/system.hpp"

#include "watch/watch.hpp"
#include "watch/power.hpp"

TTGOClass *watch;

TaskHandle_t buttonListenerTaskHandle = NULL;
SemaphoreHandle_t lastShortPressTimestampMutex = NULL;
time_t lastShortPressTimestamp = 0;

void showClock(void *p)
{
    auto tnow = watch->rtc->formatDateTime(PCF_TIMEFORMAT_HMS);
    watch->tft->setCursor(8, 8);
    watch->tft->setTextSize(3);
    watch->tft->setTextFont(2);
    watch->tft->print(tnow);
}

PowerApi powerApi;
SystemApi systemApi;

typedef struct
{
    void *lastShortPressTimestampMutex;
    long *lastShortPressTimestamp;
    PowerApi *powerApi;
    SystemApi *systemApi;
} ButtonListenerParameters;

ButtonListenerParameters buttonListenerParameters;

void buttonListener(ButtonListenerParameters *p)
{
    p->powerApi->readIRQ();
    if (p->powerApi->isPEKShortPressIRQ())
    {
        if (p->systemApi->take(p->lastShortPressTimestampMutex, 100))
        {
            Serial.println("PowerKey Press");
            p->systemApi->give(p->lastShortPressTimestampMutex);
        }
    }
    p->powerApi->clearIRQ();
}

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