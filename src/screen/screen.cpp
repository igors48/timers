#include <Arduino.h>

#include "screen.hpp"

#include "component/batteryDisplayComponent.hpp"
#include "component/hourMinuteComponent.hpp"
#include "component/secondComponent.hpp"
#include "component/stepCounterComponent.hpp"
#include "component/dateComponent.hpp"
#include "component/buttonComponent.hpp"

HourMinuteComponentState hourMinuteComponentState;
Component hourMinuteComponent;
SecondComponentState secondComponentState;
Component secondComponent;
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

Component* mount(Component *component)
{
    (component->mount)(component, 0, 0);
    return component;
}

void createComponents(void* components[])
{
    batteryDisplayComponentState = {
        ._battPercentage = -1,
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
    dateComponent = createDateComponent(60, 175, 50, 50, &dateComponentState);
    stepCounterComponent = createStepCounterComponent(35, 150, 50, 50, &stepCounterComponentState);
    hourMinuteColorChangeButton = createButtonComponent(10, 20, 66, 25, &hourMinuteColorChangeButtonState);
    secondColorChangeButton = createButtonComponent(160, 20, 66, 25, &secondColorChangeButtonState);

    components[0] = mount(&hourMinuteComponent);
    components[1] = mount(&secondComponent);
    components[2] = mount(&batteryDisplayComponent);
    components[3] = mount(&dateComponent);
    components[4] = mount(&stepCounterComponent);
    components[5] = mount(&hourMinuteColorChangeButton);
    components[6] = mount(&secondColorChangeButton);
}