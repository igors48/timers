/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Motor API implementation
*/
#include "motor.hpp"
#include "ttgo.hpp"

/**
 * @brief default instance of Motor class
 * 
 * This is needed because TTGO library motor implementation conflicts with the backlight.
 * Fix based on the PR - https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/pull/146
 * 
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