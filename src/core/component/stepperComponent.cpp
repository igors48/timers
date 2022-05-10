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

static void onPlus()
{
    Serial.println("plus");
}

static void onMinus()
{
    Serial.println("minus");
}

StepperComponentState createStepperComponentState(signed short min, signed short max, signed short value, OnStepperChange onChange)
{
    ButtonComponentState *plusButtonState = createButtonStateRef(PLUS, EG_REPEAT, onPlus);
    ButtonComponentState *minusButtonState = createButtonStateRef(MINUS, EG_REPEAT, onMinus);

    Component plusButton = createButtonComponent(0, 0, 50, 50, plusButtonState);
    Component minusButton = createButtonComponent(0, 55, 50, 50, minusButtonState);

    return {
        .min = 0,
        .max = 10,
        .value = 5,
        .plusButton = plusButton,
        .minusButton = minusButton,
        .onChange = onChange,
    };
    //GroupState state = createGroupState(2, children);
    //Component group;
}

Component createStepperComponent(signed short x, signed short y, StepperComponentState *state)
{
    pdArray = ( double * ) pvPortMalloc( xArraySize * sizeof( double ) );
}