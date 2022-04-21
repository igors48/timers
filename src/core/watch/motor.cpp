#include "motor.hpp"
#include "ttgo.hpp"

void motorBuzz(int duration)
{
    watch->motor->onec(duration);
}

MotorApi watchMotorApi()
{
    return {
        .buzz = motorBuzz,
    };
}