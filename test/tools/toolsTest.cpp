#include <unity.h>

#include "tools/tools.cpp"

void smoke()
{
    microsToNextHour();
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(smoke);
    UNITY_END();
}