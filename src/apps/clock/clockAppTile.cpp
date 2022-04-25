#include "clockAppTile.hpp"

#include "core/component/textComponent.hpp"
#include "core/component/group.hpp"

static const unsigned char COMPONENTS_COUNT = 2;
static void* components[COMPONENTS_COUNT];

static TextState hourMinute;
static Component hourMinuteComponent;

static TextState second;
static Component secondComponent;

static GroupState state;
static Component group;

Component* createClockAppTile()
{
    hourMinute = createTextState(7, 1, COLOR_INFORMATION, provideHourMinuteState);
    second = createTextState(7, 1, COLOR_INFORMATION, provideSecondState);

    hourMinuteComponent = createTextComponent(10, 90, 140, 48, &hourMinute);
    secondComponent = createTextComponent(150, 90, 75, 48, &second);

    components[0] = &hourMinuteComponent;
    components[1] = &secondComponent;
 
    state = createGroupState(COMPONENTS_COUNT, components);
    group = createGroupComponent(0, 0, &state);

    return &group;
}