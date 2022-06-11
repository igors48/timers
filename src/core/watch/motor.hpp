/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Motor API declaration
*/
#pragma once

/**
 * @brief Motor API functions
 */
typedef struct {
    /**
     * @brief makes a buzz
     * 
     * @param duration duration of a buzz
     */
    void (*buzz)(int duration);    
} MotorApi;

/**
 * @brief creates default Motor API instance
 * 
 * @return MotorApi Motor API instance
 */
MotorApi watchMotorApi();
