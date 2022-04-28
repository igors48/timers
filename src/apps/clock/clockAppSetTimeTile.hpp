#pragma once

#include "core/component/component.hpp"
#include "core/watch/rtc.hpp"

#include "apps/clock/clockApp.hpp"

Component* createClockAppSetTimeTile(Date *dateRef, OnGesture onGestureRef);