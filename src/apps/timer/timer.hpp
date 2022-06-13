/**
 * @file 
 * @author Igor Usenko (github: igors48) 
 * @brief timer declarations
*/
#pragma once

#include "apps/timer/timeKeeper.hpp"
#include "core/system/sound.hpp"

/**
 * @brief timer responses
 */
typedef enum {
    /**
     * @brief requests successfully handled by timer
     */
    TMR_OK,
    /**
     * @brief error while timer request handling 
     */
    TMR_ERROR,
} TimerResponse ;

/**
 * @brief timer statuses
 */
typedef enum {
    /**
     * @brief timer stopped
     */
    TMS_IDLE,
    /**
     * @brief timer counting down
     */
    TMS_RUN,
    /**
     * @brief timer making beeps
     */
    TMS_ALARM,
} TimerState;

/**
 * @brief timer context data
 */
typedef struct
{
    /**
     * @brief time keeper for time interval handling
     */
    TimeKeeper timeKeeper;
    /**
     * @brief alarm duration in milliseconds
     */
    unsigned int alarmDuration;
    /**
     * @brief tick of the last alarm beep
     */
    unsigned int lastBeep;
    /**
     * @brief current state of the timer
     */
    TimerState state;
    /**
     * @brief pointer to the sound API 
     */
    SoundApi *soundApi;
} Timer;

/**
 * @brief starts timer for given duration
 * 
 * @param timer pointer to timer
 * @param duration needed duration
 * @param tickCount start tick count value
 * @return TimerResponse TimerResponse.TMR_OK if no error TimerResponse.TMR_ERROR otherwise
 */
TimerResponse timerStart(Timer *timer, unsigned long duration, unsigned long tickCount);

/**
 * @brief stops the timer 
 * 
 * @param timer pointer to timer 
 * @return TimerResponse TimerResponse.TMR_OK if no error TimerResponse.TMR_ERROR otherwise
 */
TimerResponse timerStop(Timer *timer);

/**
 * @brief sends tick to the timer
 * 
 * @param timer pointer to timer  
 * @param tickCount tick count value
 */
void timerTick(Timer *timer, unsigned long tickCount); 

/**
 * @brief factory method for create timer
 * 
 * @param soundApi pointer to the sound API 
 * @return Timer new timer instance
 */
Timer timerCreate(SoundApi *soundApi);


