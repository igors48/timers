#pragma once

#include "system/sound.hpp"
#include "component/component.hpp"

typedef void (*SetActiveTile)(unsigned short activeTile);

Component createScreen(SoundApi *soundApi);