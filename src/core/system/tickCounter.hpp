/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief System tick counter based on unsigned long type
 * 
 * FreeRTOS uses unsigned int for tick counter  
 * That means FreeRTOS tick counter overflows after 49 days  
 * These functions handles unsigned long counter based on the FreeRTOS one but with overflow check  
*/
#pragma once

/**
 * @brief Calculates new tick counter value based on value received from FreeRTOS tick counter 
 * 
 * Detects overflow by comparing value with previously stored one  
 * If value lasser that previous it means that overflow happened
 * 
 * @param value value obtained from FreeRTOS
 * @return new tick counter value 
 */
unsigned long newTickCountValue(unsigned int value);

/**
 * @brief Resets tick counter to the initial value
 * 
 * @param value initial value
 */
void resetTickCount(unsigned int value);