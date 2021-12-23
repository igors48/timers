#pragma once

#include "../system/system.hpp"
#include "../watch/i2s.hpp"

typedef struct 
{
    void *queue;
    I2sApi *i2sApi;
    SystemApi *systemApi;    
} SoundPlayerParameters;

void soundPlayer(SoundPlayerParameters *p);