#include <cstdio>
#include <unity.h>
#include "backlightController.cpp"

int backlightLevelMutex = 0;
unsigned char backlightLevel = 0;

void setBrightness(uint8_t level) {
}

void log(const char *message) {
    printf("%s \r", message);
}

bool give(void *semaphore) {
    return true;
}

bool take(void *semaphore, unsigned int blockTime) {
    return true;
}

int main() {
    UNITY_BEGIN();
    BackligthControllerParameters p = {
        .backlightLevelMutex = &backlightLevelMutex,
        .backlightLevel = &backlightLevel,
        .setBrightness = setBrightness,
        .take = take,
        .give = give,
        .log = log};
    backlightController(&p);
    TEST_ASSERT_TRUE(1);
    UNITY_END();
}
