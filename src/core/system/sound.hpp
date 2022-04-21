#pragma once

#include "system.hpp"

typedef void (*Beep)();

typedef struct {
    Beep beep;
} SoundApi;

SoundApi watchSoundApi(void *queueRef, SystemApi *systemApiRef);