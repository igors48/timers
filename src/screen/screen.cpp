#include <Arduino.h>

#include "screen.hpp"

#include "component/batteryDisplayComponent.hpp"
#include "component/touchDisplayComponent.hpp"
#include "component/hourMinuteComponent.hpp"
#include "component/secondComponent.hpp"
#include "component/stepCounterComponent.hpp"
#include "component/dateComponent.hpp"
#include "component/buttonComponent.hpp"

HourMinuteComponentState hourMinuteComponentState;
SecondComponentState secondComponentState;
TouchDisplayComponentState touchDisplayComponentState;
BatteryDisplayComponentState batteryDisplayComponentState;
StepCounterComponentState stepCounterComponentState;
DateComponentState dateComponentState;
ButtonComponentState hourMinuteColorChangeButton;
ButtonComponentState secondColorChangeButton;

void changeHourMinuteColor()
{
    hourMinuteComponentColorChange(&hourMinuteComponentState);
}

void changeSecondColor()
{
    secondComponentColorChange(&secondComponentState);
}

void createComponents(Component components[])
{
    batteryDisplayComponentState = {
        ._battPercentage = -1,
    };

    touchDisplayComponentState = {
        ._touchX = -1,
        ._touchY = -1,
    };

    hourMinuteComponentState = {
        .color = COLOR_INFORMATION,
        ._color = 0,
        ._hour = 0,
        ._minute = 0,
    };

    secondComponentState = {
        .color = COLOR_INFORMATION,
        ._color = 0,
        ._second = 0,
    };

    stepCounterComponentState = {
        ._stepCount = -1,
    };

    dateComponentState = {
        ._year = 0,
        ._month = 0,
        ._day = 0,
    };

    hourMinuteColorChangeButton = {
        .pressed = false,
        ._pressed = true,
        .handler = changeHourMinuteColor
    };

    secondColorChangeButton = {
        .pressed = false,
        ._pressed = true,
        .handler = changeSecondColor
    };

    components[0] = createHourMinuteComponent(10, 90, 140, 48, &hourMinuteComponentState);
    components[1] = createSecondComponent(150, 90, 75, 48, &secondComponentState);
    components[2] = createBatteryDisplayComponent(135, 150, 50, 50, &batteryDisplayComponentState);
    components[3] = createTouchDisplayComponent(0, 232, 200, 50, &touchDisplayComponentState);
    components[4] = createDateComponent(60, 175, 50, 50, &dateComponentState);
    components[5] = createStepCounterComponent(35, 150, 50, 50, &stepCounterComponentState);
    components[6] = createButtonComponent(10, 20, 66, 25, &hourMinuteColorChangeButton);
    components[7] = createButtonComponent(160, 20, 66, 25, &secondColorChangeButton);
}