/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Ticker task declarations
 * 
 * Ticker it is a task which periodically calls a function 
 * with the configured delay between calls
*/
#pragma once

#include "core/system/system.hpp"

/**
 * @brief Ticker parameters
 */
typedef struct {
    /**
     * @brief Ticker name
     */
    char *name;
    /**
     * @brief Pointer to the watch mutex
     */
    void *watchMutex;
    /**
     * @brief Delay between function calls in milliseconds
     */
    unsigned int delayMs;
    /**
     * @brief Pointer to the function called by ticker
     * 
     * @param tick current FreeRTOS tick count
     */
    void (*func)(unsigned long tick);
    /**
     * @brief Pointer to the system API
     */
    SystemApi *systemApi;
} TickerParameters;

/**
 * @brief Ticker task function
 * 
 * This function is calling from the FreeRTOS task
 * 
 * @param p Pointer to the ticker parameters 
 */
void tickerTask(void *p);
