#include <string.h>

#include "factory.hpp"

static SystemApi *systemApi;

static GroupState* createGroupStateRef(unsigned char childrenCount, void **children)
{
    GroupState *state = (GroupState *)(systemApi->allocate)(sizeof(GroupState));    

    state->childrenCount = childrenCount;
    state->children = children;

    return state;
}

static Component* createGroupComponentRef(signed short x, signed short y, GroupState *state)
{
    Component *component = (Component *)(systemApi->allocate)(sizeof(Component));

    component->x = x;
    component->y = y;
    component->w = 0;
    component->h = 0;
    component->contains = groupContains;
    component->mount = groupMount;
    component->onTouch = componentNoopHandler;
    component->onMove = componentNoopHandler;
    component->onRelease = componentNoopHandler;
    component->onGesture = componentGestureNoopHandler;
    component->render = groupRender;
    component->newState = groupNewState;
    component->state = state;

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
    Component* ref = (Component *)(systemApi->allocate)(sizeof(Component));

    ref->x = x;
    ref->y = y;
    ref->w = w;
    ref->h = h;
    ref->contains = componentContains;
    ref->mount = componentMount;
    ref->onTouch = componentNoopHandler;
    ref->onMove = componentNoopHandler;
    ref->onRelease = componentNoopHandler;
    ref->onGesture = componentGestureNoopHandler;
    ref->render = textComponentRender;
    ref->newState = textComponentNewState;
    ref->state = state;

    return ref;
}

static ButtonComponentState* createButtonStateRef(char *title, EventGenerate eventGenerate, Handler handler)
{
    ButtonComponentState *state = (ButtonComponentState *)(systemApi->allocate)(sizeof(ButtonComponentState));
    
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

static Component* createButtonComponentRef(signed short x, signed short y, signed short w, signed short h, ButtonComponentState *state)
{
    Component *component = (Component *)(systemApi->allocate)(sizeof(Component));

    component->x = x;
    component->y = y;
    component->w = w;
    component->h = h;
    component->contains = componentContains;
    component->mount = componentMount;
    component->onTouch = buttonOnTouch;
    component->onMove = buttonOnMove;
    component->onRelease = buttonOnRelease;
    component->onGesture = componentGestureNoopHandler;
    component->render = buttonRender;
    component->newState = buttonNewState;
    component->state = state;

    return component;
}

char STEPPER_PLUS[] = "+"; // todo find better place
char STEPPER_MINUS[] = "-";

static StepperComponentState* createStepperComponentStateRef(signed short min, signed short max, signed short value, OnStepperChange onChange)
{
    ButtonComponentState *plusButtonState = createButtonStateRef(STEPPER_PLUS, EG_REPEAT, stepperOnPlus);
    ButtonComponentState *minusButtonState = createButtonStateRef(STEPPER_MINUS, EG_REPEAT, stepperOnMinus);

    Component *plusButton = createButtonComponentRef(0, 0, 50, 50, plusButtonState);
    Component *minusButton = createButtonComponentRef(0, 55, 50, 50, minusButtonState);

    StepperComponentState *state = (StepperComponentState *)(systemApi->allocate)(sizeof(StepperComponentState));

    state->min = 0;
    state->max = 10;
    state->value = 5;
    state->plusButton = plusButton;
    state->minusButton = minusButton;
    state->onChange = onChange;

    plusButtonState->context = (void *)state;
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