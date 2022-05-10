#include "stepperComponent.hpp"
#include "buttonComponent.hpp"
#include "group.hpp"

#include <LilyGoWatch.h>

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

StepperComponentState createStepperComponentState(signed short min, signed short max, signed short value, OnStepperChange onChange)
{
    ButtonComponentState *plusButtonState = (ButtonComponentState *)pvPortMalloc(sizeof(ButtonComponentState));
    Component plusButton;

    ButtonComponentState minusButtonState;
    Component minusButton;

    GroupState state;
    Component group;
}
