#pragma once

typedef void (*Beep)();

typedef struct {
    Beep beep;
} SoundApi;

SoundApi watchSoundApi(void *queueRef, SystemApi *systemApiRef);