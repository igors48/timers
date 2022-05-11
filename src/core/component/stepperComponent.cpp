#include "stepperComponent.hpp"
#include "buttonComponent.hpp"
#include "group.hpp"

#include <LilyGoWatch.h>

static char PLUS[] = "+";
static char MINUS[] = "-";

static ButtonComponentState* createButtonStateRef(char *title, EventGenerate eventGenerate, Handler handler)
{
    ButtonComponentState *state = (ButtonComponentState *)pvPortMalloc(sizeof(ButtonComponentState));
    
    state->title = title;
    state->eventGenerate = eventGenerate;
    state->handler = handler;
    state->delayTick = 1000; // todo pass as a parameter. depends on portTICK_PERIOD_MS
    state->repeatTick = 250; // todo pass as a parameter. depends on portTICK_PERIOD_MS
    state->eventHandlingState = EHS_IDLE;
    state->_eventHandlingState = EHS_INIT;
    state->mode = BM_ENABLED;
    state->_mode = BM_INIT;
    state->firstTouchTick = 0;
    state->lastRepeatTick = 0;
    
    return state;
}

static GroupState* createGroupStateRef(unsigned char childrenCount, void **children)
{
    GroupState *state = (GroupState *)pvPortMalloc(sizeof(GroupState));    

    state->childrenCount = childrenCount;
    state->children = children;

    return state;
}

static void onPlus()
{
    Serial.println("plus");
}

static void onMinus()
{
    Serial.println("minus");
}

StepperComponentState* createStepperComponentStateRef(signed short min, signed short max, signed short value, OnStepperChange onChange)
{
    ButtonComponentState *plusButtonState = createButtonStateRef(PLUS, EG_REPEAT, onPlus);
    ButtonComponentState *minusButtonState = createButtonStateRef(MINUS, EG_REPEAT, onMinus);

    Component plusButton = createButtonComponent(0, 0, 50, 50, plusButtonState);
    Component minusButton = createButtonComponent(0, 55, 50, 50, minusButtonState);

    StepperComponentState *state = (StepperComponentState *)pvPortMalloc(sizeof(StepperComponentState));

    state->min = 0;
    state->max = 10;
    state->value = 5;
    state->plusButton = plusButton;
    state->minusButton = minusButton;
    state->onChange = onChange;

    return state;
}

Component* createStepperComponentRef(signed short x, signed short y, StepperComponentState *state)
{
    void** components = (void**)pvPortMalloc(2 * sizeof(void*));
    components[0] = &(state->plusButton);
    components[1] = &(state->minusButton);

    GroupState* groupState = createGroupStateRef(2, components);
    return createGroupComponentRef(0, 0, groupState);
}