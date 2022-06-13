/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Structure and functions for handling time intervals based on regular ticks.
*/ 
#pragma once

/**
 * @brief defines parameters needed for handling time interval
*/
typedef struct {
    unsigned long duration; /**< count of ticks to the end of interval. zero means that interval is passed */
    unsigned long lastTick; /**< value of latest handled tick */
} TimeKeeper;

/**
 * @brief handles tick
 * 
 * decreaces duration according to received tick count, 
 * stores received tick count as a last tick. 
 * if the rest of the duration is lesser than 0 then sets duration to zero.
 * 
 * @param timeKeeper pointer to time keeper
 * @param tickCount tick count value
*/
void timeKeeperTick(TimeKeeper *timeKeeper, unsigned long tickCount); 

/**
 * @brief resets time keeper to initial state
 * 
 * sets duration and tickCount to values of a corresponding parameters
 * 
 * @param timeKeeper pointer to time keeper
 * @param duration  duration value
 * @param tickCount tick count value
*/
void timeKeeperReset(TimeKeeper *timeKeeper, unsigned long duration, unsigned long tickCount);

/**
 * @brief factory method for time keeper creation
*/ 
TimeKeeper timeKeeperCreate();