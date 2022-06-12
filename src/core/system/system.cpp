/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief implementation for the default system API
*/
#include <Arduino.h>
#include <time.h>
#include "system.hpp"
#include "tickCounter.hpp"

/**
 * @copydoc SystemApi.log
 */
void systemLog(const char *source, const char *message, ...)
{
    char buf[256];
    va_list args;
    va_start(args, message);
    vsprintf(buf, message, args);
    va_end(args);
    Serial.printf("%s: %s \r\n", source, buf);
}

/**
 * @copydoc SystemApi.give
 */
bool systemGive(void *semaphore)
{
    return xSemaphoreGive(*(SemaphoreHandle_t *)semaphore); // should be exactly *(SemaphoreHandle_t *)semaphore
    //return xSemaphoreGive((SemaphoreHandle_t *)semaphore);
}

/**
 * @copydoc SystemApi.take
 */
bool systemTake(void *semaphore, unsigned int blockTimeMillis)
{
    return xSemaphoreTake(*(SemaphoreHandle_t *)semaphore, (TickType_t)blockTimeMillis / portTICK_PERIOD_MS); // should be exactly *(SemaphoreHandle_t *)semaphore
    //return xSemaphoreTake((SemaphoreHandle_t *)semaphore, (TickType_t)blockTime / portTICK_PERIOD_MS);
}

/**
 * @copydoc SystemApi.time
 */
long systemTime()
{
    return time(NULL);
}

/**
 * @copydoc SystemApi.delay
 */
void systemDelay(unsigned int time)
{
    vTaskDelay(time / portTICK_PERIOD_MS);
}

/**
 * @copydoc SystemApi.suspend
 */
void systemSuspend(void *handle)
{
    vTaskSuspend(handle);
}

/**
 * @copydoc SystemApi.resume
 */
void systemResume(void *handle)
{
    vTaskResume(handle);
}

/**
 * @copydoc SystemApi.queueReceive
 */
bool systemQueueReceive(void *queue, void *buffer, unsigned int blockTimeMillis)
{
    return xQueueReceive(*(QueueHandle_t *)queue, buffer, (TickType_t)blockTimeMillis / portTICK_PERIOD_MS) == pdTRUE;   
}

/**
 * @copydoc SystemApi.queueSend
 */
bool systemQueueSend(void *queue, void *item, unsigned int blockTimeMillis)
{
    return xQueueSend(*(QueueHandle_t *)queue, item, (TickType_t)blockTimeMillis / portTICK_PERIOD_MS) == pdTRUE;   
}

/**
 * @copydoc SystemApi.getTickCount
 */
unsigned long systemGetTickCount()
{
    const unsigned int tick = xTaskGetTickCount();
    return newTickCountValue(tick);
}

/**
 * @copydoc SystemApi.allocate
 */
void* systemAllocate(unsigned int size)
{
    return pvPortMalloc(size);
}

SystemApi defaultSystemApi()
{
    const unsigned int startTick = xTaskGetTickCount(); 
    resetTickCount(startTick);

    return {
        .take = systemTake,
        .give = systemGive,
        .log = systemLog,
        .time = systemTime,
        .delay = systemDelay,
        .suspend = systemSuspend,
        .resume = systemResume,
        .queueReceive = systemQueueReceive,
        .queueSend = systemQueueSend,
        .getTickCount = systemGetTickCount,
        .allocate = systemAllocate, // todo replace calls with pointers where possible in the all APIs
    };
}