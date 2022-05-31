#pragma once

typedef struct {
    unsigned int duration;
    unsigned int lastTick; 
} TimeKeeper;

void timeKeeperTick(TimeKeeper *timeKeeper, unsigned int tickCount); 
void timeKeeperReset(TimeKeeper *timeKeeper, unsigned int duration, unsigned int tickCount);

TimeKeeper timeKeeperCreate();