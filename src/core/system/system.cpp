#include <Arduino.h>
#include <time.h>
#include "system.hpp"

void systemLog(const char *source, const char *message, ...)
{
    char buf[256];
    va_list args;
    va_start(args, message);
    vsprintf(buf, message, args);
    va_end(args);
    Serial.printf("%s: %s \r\n", source, buf);
}

bool systemGive(void *semaphore)
{
    return xSemaphoreGive(*(SemaphoreHandle_t *)semaphore); // should be exactly *(SemaphoreHandle_t *)semaphore
    //return xSemaphoreGive((SemaphoreHandle_t *)semaphore);
}

bool systemTake(void *semaphore, unsigned int blockTimeMillis)
{
    return xSemaphoreTake(*(SemaphoreHandle_t *)semaphore, (TickType_t)blockTimeMillis / portTICK_PERIOD_MS); // should be exactly *(SemaphoreHandle_t *)semaphore
    //return xSemaphoreTake((SemaphoreHandle_t *)semaphore, (TickType_t)blockTime / portTICK_PERIOD_MS);
}

long systemTime()
{
    return time(NULL);
}

void systemDelay(unsigned int time)
{
    vTaskDelay(time / portTICK_PERIOD_MS);
}

void systemDelayUntil(unsigned int *prevoiusWakeTimeMillis, unsigned int timeIncrementMillis)
{
    vTaskDelayUntil(prevoiusWakeTimeMillis, timeIncrementMillis / portTICK_PERIOD_MS);
}

void systemSuspend(void *handle)
{
    vTaskSuspend(handle);
}

void systemResume(void *handle)
{
    vTaskResume(handle);
}

bool systemQueueReceive(void *queue, void *buffer, unsigned int blockTimeMillis)
{
    return xQueueReceive(*(QueueHandle_t *)queue, buffer, (TickType_t)blockTimeMillis / portTICK_PERIOD_MS) == pdTRUE;   
}

bool systemQueueSend(void *queue, void *item, unsigned int blockTimeMillis)
{
    return xQueueSend(*(QueueHandle_t *)queue, item, (TickType_t)blockTimeMillis / portTICK_PERIOD_MS) == pdTRUE;   
}

unsigned int systemGetTickCount()
{
    return xTaskGetTickCount();
}

void* systemAllocate(unsigned int size)
{
    return pvPortMalloc(size);
}

SystemApi defaultSystemApi()
{
    return {
        .take = systemTake,
        .give = systemGive,
        .log = systemLog,
        .time = systemTime,
        .delay = systemDelay,
        .delayUntil = systemDelayUntil,
        .suspend = systemSuspend,
        .resume = systemResume,
        .queueReceive = systemQueueReceive,
        .queueSend = systemQueueSend,
        .getTickCount = systemGetTickCount,
        .allocate = systemAllocate, // todo replace calls with pointers where possible in the all APIs
    };
}