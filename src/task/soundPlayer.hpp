#pragma once

#include "../system/system.hpp"

typedef struct 
{
    void *queue;
    SystemApi *systemApi;    
} SoundPlayerParameters;

void soundPlayer(SoundPlayerParameters *p);