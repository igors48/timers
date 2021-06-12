#include <Arduino.h>

void log(const char *source, const char *message, ...)
{
    char buf[256];
    va_list args;
    va_start(args, message);
    vsprintf(buf, message, args);
    va_end(args);
    Serial.printf("%s: %s \r\n", source, buf);
}

bool give(void *semaphore)
{
    return xSemaphoreGive(*(SemaphoreHandle_t *)semaphore); // should be exactly *(SemaphoreHandle_t *)semaphore
}

bool take(void *semaphore, unsigned int blockTime)
{
    return xSemaphoreTake(*(SemaphoreHandle_t *)semaphore, (TickType_t)blockTime / portTICK_PERIOD_MS); // should be exactly *(SemaphoreHandle_t *)semaphore
}

long time()
{
    return time(NULL);
}

void frDelay(unsigned int time)
{
    vTaskDelay(time / portTICK_PERIOD_MS);
}