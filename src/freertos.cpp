#include <Arduino.h>

void log(const char *message) {
    Serial.println(message);
}

bool give(void *semaphore) {
    return xSemaphoreGive(*(SemaphoreHandle_t *)semaphore);
}

bool take(void *semaphore, unsigned int blockTime) {
    Serial.println("take");
    return xSemaphoreTake(*(SemaphoreHandle_t *)semaphore, ( TickType_t )blockTime/* / portTICK_PERIOD_MS*/);
}