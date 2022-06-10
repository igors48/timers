/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Motor API implementation
*/
#include "motor.hpp"
#include "ttgo.hpp"

/**
 * @brief default instance of Motor class
 */
static Motor motor = Motor(MOTOR_PIN, 4, 100);

/**
 * @copydoc MotorApi.buzz
 */
static void buzz(int duration)
{
    motor.onec(duration);
}

MotorApi watchMotorApi()
{
    motor.begin();

    return {
        .buzz = buzz,
    };
}