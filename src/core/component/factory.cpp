#include <string.h>

#include "factory.hpp"

static SystemApi *systemApi;

static Component* createComponentRef(signed short x, signed short y, signed short w, signed short h, void *state)
{
    Component *component = (Component *)(systemApi->allocate)(sizeof(Component));
    *component = createComponent(x, y, w, h, state);
    return component;
}

static GroupState* createGroupStateRef(unsigned char childrenCount, void **children)
{
    GroupState *state = (GroupState *)(systemApi->allocate)(sizeof(GroupState));    

    state->childrenCount = childrenCount;
    state->children = children;
    state->onTick = groupNoopTick;

    return state;
}

static Component* createGroupComponentRef(signed short x, signed short y, GroupState *state)
{
    Component *component = createComponentRef(x, y, 0, 0, state);

    component->contains = groupContains;
    component->mount = groupMount;
    component->isStateModified = groupIsStateModified;
    component->updateState = groupUpdateState;
    component->render = groupRender;

    return component;    
}

static TextState* createTextStateRef(unsigned char font, unsigned char size, unsigned int fontColor, Provide provide)
{
    TextState *ref = (TextState *)(systemApi->allocate)(sizeof(TextState));

    strcpy(ref->content, "");
    strcpy(ref->_content, "_");
    ref->size = size;
    ref->_size = 0;
    ref->fontColor = fontColor;
    ref->_fontColor = 0;
    ref->backColor = COLOR_BLACK;
    ref->_backColor = 0;
    ref->font = font;
    ref->_font = 0;
    ref->provide = provide;

    return ref;
}

static Component* createTextComponentRef(signed short x, signed short y, signed short w, signed short h, TextState *state)
{
    Component* ref = createComponentRef(x, y, w, h, state);

    ref->render = textComponentRender;
    ref->isStateModified = textComponentIsStateModified;
    ref->updateState = textComponentUpdateState;

    return ref;
}

static ButtonComponentState* createButtonStateRef(char *title, EventGenerate eventGenerate, Handler handler)
{
    ButtonComponentState *state = (ButtonComponentState *)(systemApi->allocate)(sizeof(ButtonComponentState));
    
    state->title = title;
    state->eventGenerate = eventGenerate;
    state->handler = handler;
    state->delayTick = 1000; // todo pass as a parameter. depends on portTICK_PERIOD_MS #120
    state->repeatTick = 250; // todo pass as a parameter. depends on portTICK_PERIOD_MS #120
    state->eventHandlingState = EHS_IDLE;
    state->_eventHandlingState = EHS_INIT;
    state->mode = BM_ENABLED;
    state->_mode = BM_INIT;
    state->firstTouchTick = 0;
    state->lastRepeatTick = 0;
    
    return state;
}

static Component* createButtonComponentRef(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state)
{
    Component *component = createComponentRef(x, y, w, h, state);

    component->onTouch = buttonOnTouch;
    component->onMove = buttonOnMove;
    component->onRelease = buttonOnRelease;
    component->render = buttonRender;
    component->isStateModified = buttonIsStateModified;
    component->updateState = buttonUpdateState;

    return component;
}

char STEPPER_PLUS[] = "+"; // todo find better place #119
char STEPPER_MINUS[] = "-";

static StepperComponentState* createStepperComponentStateRef(signed short min, signed short max, signed short value, OnStepperChange onChange)
{
    ButtonComponentState *plusButtonState = createButtonStateRef(STEPPER_PLUS, EG_REPEAT, stepperOnPlus);
    ButtonComponentState *minusButtonState = createButtonStateRef(STEPPER_MINUS, EG_REPEAT, stepperOnMinus);

    Component *plusButton = createButtonComponentRef(0, 0, 50, 50, plusButtonState);
    Component *minusButton = createButtonComponentRef(0, 55, 50, 50, minusButtonState);

    StepperComponentState *state = (StepperComponentState *)(systemApi->allocate)(sizeof(StepperComponentState));

    state->min = min;
    state->max = max;
    state->value = value;
    state->plusButton = plusButton;
    state->minusButton = minusButton;
    state->onChange = onChange;
    state->setValue = stepperSetValue;

    plusButtonState->context = (void *)state; // todo add it to the createButtonStateRef #118
    minusButtonState->context = (void *)state;

    return state;
}

static Component* createStepperComponentRef(signed short x, signed short y, StepperComponentState *state)
{
    void** components = (void**)(systemApi->allocate)(2 * sizeof(void*));
    components[0] = state->plusButton;
    components[1] = state->minusButton;

    GroupState* groupState = createGroupStateRef(2, components);
    return createGroupComponentRef(x, y, groupState);
}

Factory createFactory(SystemApi *systemApiRef)
{
    systemApi = systemApiRef;

    return {
        .createGroupStateRef = createGroupStateRef,
        .createGroupComponentRef = createGroupComponentRef,
        .createTextStateRef = createTextStateRef,
        .createTextComponentRef = createTextComponentRef,
        .createButtonStateRef = createButtonStateRef,
        .createButtonComponentRef = createButtonComponentRef,
        .createStepperComponentStateRef = createStepperComponentStateRef,
        .createStepperComponentRef = createStepperComponentRef,
    };
}