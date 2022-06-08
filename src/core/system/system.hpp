/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief functions and types which provides interface to the FreeRTOS functions
*/
#pragma once

/**
 * @brief set of the system API functions
 */
typedef struct
{
    /**
     * @brief takes a FreeRTOS semaphore
     */
    bool (*take)(void *semaphore, unsigned int blockTimeMillis);
    
    /**
     * @brief gives a FreeRTOS semaphore
     */
    bool (*give)(void *semaphore);
    
    /**
     * @brief writes message to the log
     * 
     * @param source name of the log message source
     * @param message log message template
     * @param ... log message data
     */
    void (*log)(const char *source, const char *message, ...);

    /**
     * @brief returns system time
     */
    long (*time)();

    /**
     * @brief initiates FreeRTOS task delay
     */
    void (*delay)(unsigned int time);

    /**
     * @brief suspends FreeRTOS task 
     * 
     * @param handle task handle
     */
    void (*suspend)(void *handle);

    /**
     * @brief resumes FreeRTOS task 
     * 
     * @param handle task handle
     */
    void (*resume)(void *handle);
    
    /**
     * @brief receives an item from FreeRTOS queue 
     */
    bool (*queueReceive)(void *queue, void *buffer, unsigned int blockTimeMillis);

    /**
     * @brief sends an item to FreeRTOS queue
     */
    bool (*queueSend)(void *queue, void *item, unsigned int blockTimeMillis);
    
    /**
     * @brief returns FreeRTOS tick count
     */
    unsigned int (*getTickCount)();

    /**
     * @brief allocates a memory block
     */
    void* (*allocate)(unsigned int size);
} SystemApi;

/**
 * @brief factory function for creation default system API 
*/
SystemApi defaultSystemApi();