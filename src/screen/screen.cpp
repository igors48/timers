#include <Arduino.h>

#include "screen.hpp"

#include "component/batteryDisplayComponent.hpp"
#include "component/touchDisplayComponent.hpp"
#include "component/hourMinuteComponent.hpp"
#include "component/secondComponent.hpp"
#include "component/stepCounterComponent.hpp"
#include "component/dateComponent.hpp"
#include "component/changeColorComponent.hpp"

HourMinuteComponentState hourMinuteComponentState;
SecondComponentState secondComponentState;
TouchDisplayComponentState touchDisplayComponentState;
BatteryDisplayComponentState batteryDisplayComponentState;
StepCounterComponentState stepCounterComponentState;
DateComponentState dateComponentState;
ChangeColorComponentState changeColorComponentState;

void changeColor()
{
    hourMinuteComponentColorChange(&hourMinuteComponentState);
    Serial.println("changeColor");
}

void createComponents(Component components[])
{
    batteryDisplayComponentState = {
        ._battPercentage = 0, // todo set to -1
    };

    touchDisplayComponentState = {
        ._touchX = -1,
        ._touchY = -1,
    };

    hourMinuteComponentState = {
        .color = 0xFFFF,
        ._color = 0,
        ._hour = 0,
        ._minute = 0,
    };

    secondComponentState = {
        .color = 0xFFFF,
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

    changeColorComponentState = {
        .handler = changeColor
    };

    components[0] = createHourMinuteComponent(10, 90, 140, 48, &hourMinuteComponentState);
    components[1] = createSecondComponent(150, 90, 75, 48, &secondComponentState);
    components[2] = createBatteryDisplayComponent(135, 150, 50, 50, &batteryDisplayComponentState);
    components[3] = createTouchDisplayComponent(0, 232, 200, 50, &touchDisplayComponentState);
    components[4] = createDateComponent(60, 175, 50, 50, &dateComponentState);
    components[5] = createStepCounterComponent(35, 150, 50, 50, &stepCounterComponentState);
    components[6] = createChangeColorComponent(10, 40, 50, 50, &changeColorComponentState);
}