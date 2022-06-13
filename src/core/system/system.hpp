/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Functions and types which provides interface to the FreeRTOS functions
*/
#pragma once

/**
 * @brief Set of the system API functions
 */
typedef struct
{
    /**
     * @brief Takes a FreeRTOS semaphore
     */
    bool (*take)(void *semaphore, unsigned int blockTimeMillis);
    
    /**
     * @brief Gives a FreeRTOS semaphore
     */
    bool (*give)(void *semaphore);
    
    /**
     * @brief Writes message to the log
     * 
     * @param source name of the log message source
     * @param message log message template
     * @param ... log message data
     */
    void (*log)(const char *source, const char *message, ...);

    /**
     * @brief Returns system time
     */
    long (*time)();

    /**
     * @brief Initiates FreeRTOS task delay
     */
    void (*delay)(unsigned int time);

    /**
     * @brief Suspends FreeRTOS task 
     * 
     * @param handle task handle
     */
    void (*suspend)(void *handle);

    /**
     * @brief Resumes FreeRTOS task 
     * 
     * @param handle task handle
     */
    void (*resume)(void *handle);
    
    /**
     * @brief Receives an item from FreeRTOS queue 
     */
    bool (*queueReceive)(void *queue, void *buffer, unsigned int blockTimeMillis);

    /**
     * @brief Sends an item to FreeRTOS queue
     */
    bool (*queueSend)(void *queue, void *item, unsigned int blockTimeMillis);
    
    /**
     * @brief Returns FreeRTOS tick count
     */
    unsigned long (*getTickCount)();

    /**
     * @brief Allocates a memory block
     */
    void* (*allocate)(unsigned int size);
} SystemApi;

/**
 * @brief Factory function for creation default system API
 * 
 * Also initializes support for unsigned long instead unsigned int value for FreeRTOS tick counter
*/
SystemApi defaultSystemApi();