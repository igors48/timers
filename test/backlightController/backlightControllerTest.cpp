#include <cstdio>
#include <unity.h>
#include "backlightController.cpp"

int backlightLevelMutex;
unsigned char backlightLevel;
int logCount;
bool giveResult;
bool takeResult;
unsigned char brightnessLevel;
BackligthControllerParameters parameters;

void setBrightness(unsigned char level) {
    brightnessLevel = level;
}

void log(const char *message) {
    logCount++;
    printf("%s \r", message);
}

bool give(void *semaphore) {
    return giveResult;
}

bool take(void *semaphore, unsigned int blockTime) {
    return takeResult;
}

void setUp(void) {
    backlightLevelMutex = 0;
    backlightLevel = 8;
    logCount = 0;
    giveResult = true;
    takeResult = true;
    brightnessLevel = 0;

    parameters = {
        .backlightLevelMutex = &backlightLevelMutex,
        .backlightLevel = &backlightLevel,
        .setBrightness = setBrightness,
        .take = take,
        .give = give,
        .log = log};
}

void happyFlow() {
    backlightController(&parameters);
    TEST_ASSERT_EQUAL_INT(1, logCount); // message logged
    TEST_ASSERT_EQUAL_UINT8(8, brightnessLevel); // brightness level set to backlight level 
}

void whenBacklightLevelMutexCouldNotBeTaken() {
    takeResult = false;
    brightnessLevel = 48;
    backlightController(&parameters);
    TEST_ASSERT_EQUAL_INT(1, logCount); // message logged
    TEST_ASSERT_EQUAL_UINT8(48, brightnessLevel); // brightness level does not change 
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(happyFlow);
    RUN_TEST(whenBacklightLevelMutexCouldNotBeTaken);
    UNITY_END();
}
