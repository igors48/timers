#pragma once

#include "system.hpp"

typedef struct {
    void (*beep)();
} SoundApi;

SoundApi watchSoundApi(void *queueRef, SystemApi *systemApiRef);