#pragma once

typedef struct {
    unsigned int counter;
    unsigned int lastTick; 
} TimeKeeper;

void timeKeeperTick(TimeKeeper *timeKeeper, unsigned int tickCount); 
void timeKeeperReset(TimeKeeper *timeKeeper, unsigned int counter, unsigned int tickCount);