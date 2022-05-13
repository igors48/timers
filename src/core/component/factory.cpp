#include "factory.hpp"

static SystemApi *systemApi;

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

Factory createFactory(SystemApi *systemApiRef)
{
    systemApi = systemApiRef;

    return {
        .createButtonStateRef = createButtonStateRef,
        .createButtonComponentRef = createButtonComponentRef,
    };
}