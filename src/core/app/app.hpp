/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Interface of the Application
*/
#pragma once

#include <climits>

#include "core/system/system.hpp"
#include "core/component/group.hpp"

/**
 * @brief Value returned by an application if sleep mode is not allowed 
 * 
 * In some cases, an application should continue its work even if idle mode 
 * already expired. In such cases, the application should return this value 
 * as a result of getNextWakeUpPeriod function 
 */
const unsigned long NW_NO_SLEEP = 0;

/**
 * @brief Value returned by an application if it does not care about when the clock go sleep mode
 */
const unsigned long NW_DONT_CARE = UINT_MAX;

typedef struct App_
{
    /**
     * @brief Called when the application becomes active
     */
    void (*activate)();
    /**
     * @brief Called when the application becomes inactive
     */
    void (*deactivate)();
    /**
     * @brief Returns active tile of the application
     * 
     * @return pointer to the active tile
     */
    Component* (*getActiveTile)();
    /**
     * @brief Returns next wake up period requested by the application
     * 
     * Using this function an application can control goto sleep and wake up behaviour of the clock.  
     * If application returns a value for period it means that the clock should be in active mode when period passed.
     * 
     * @return milliseconds to the next wakeup or NW_DONT_CARE or NW_NO_SLEEP
     */
    unsigned long (*getNextWakeUpPeriod)();
} App;