#pragma once

#include "core/system/sound.hpp"
#include "core/component/component.hpp"

typedef void (*SetActiveTile)(unsigned short activeTile);

Component createScreen(SoundApi *soundApi);