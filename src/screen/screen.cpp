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
Component hourMinuteComponent;
SecondComponentState secondComponentState;
Component secondComponent;
TouchDisplayComponentState touchDisplayComponentState;
Component touchDisplayComponent;
BatteryDisplayComponentState batteryDisplayComponentState;
Component batteryDisplayComponent;
StepCounterComponentState stepCounterComponentState;
Component stepCounterComponent;
DateComponentState dateComponentState;
Component dateComponent;
ButtonComponentState hourMinuteColorChangeButtonState;
Component hourMinuteColorChangeButton;
ButtonComponentState secondColorChangeButtonState;
Component secondColorChangeButton;

void changeHourMinuteColor()
{
    hourMinuteComponentColorChange(&hourMinuteComponentState);
}

void changeSecondColor()
{
    secondComponentColorChange(&secondComponentState);
}

void createComponents(void* components[])
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

    hourMinuteColorChangeButtonState = {
        .pressed = false,
        ._pressed = true,
        .handler = changeHourMinuteColor
    };

    secondColorChangeButtonState = {
        .pressed = false,
        ._pressed = true,
        .handler = changeSecondColor
    };

    hourMinuteComponent = createHourMinuteComponent(10, 90, 140, 48, &hourMinuteComponentState);
    secondComponent = createSecondComponent(150, 90, 75, 48, &secondComponentState);
    batteryDisplayComponent = createBatteryDisplayComponent(135, 150, 50, 50, &batteryDisplayComponentState);
    touchDisplayComponent = createTouchDisplayComponent(0, 232, 200, 50, &touchDisplayComponentState);
    dateComponent = createDateComponent(60, 175, 50, 50, &dateComponentState);
    stepCounterComponent = createStepCounterComponent(35, 150, 50, 50, &stepCounterComponentState);
    hourMinuteColorChangeButton = createButtonComponent(10, 20, 66, 25, &hourMinuteColorChangeButtonState);
    secondColorChangeButton = createButtonComponent(160, 20, 66, 25, &secondColorChangeButtonState);

    components[0] = &hourMinuteComponent;
    components[1] = &secondComponent;
    components[2] = &batteryDisplayComponent;
    components[3] = &touchDisplayComponent;
    components[4] = &dateComponent;
    components[5] = &stepCounterComponent;
    components[6] = &hourMinuteColorChangeButton;
    components[7] = &secondColorChangeButton;
}