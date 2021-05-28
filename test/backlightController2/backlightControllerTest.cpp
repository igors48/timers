#include <unity.h>
#include "backlightController.cpp"

int main() {
    UNITY_BEGIN();
    BackligthControllerParameters p = {};
    backlightController(&p);
    TEST_ASSERT_TRUE(1);
    UNITY_END();
}
