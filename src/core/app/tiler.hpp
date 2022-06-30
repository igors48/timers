/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Application's tile controller
 * 
 * Render active Tile of the active Application  
 * Sends events received from the system to the active Tile   
 * 
*/
#pragma once

#include "core/app/app.hpp"

/**
 * @brief Tiler interface definition
 */
typedef struct 
{
    /**
     * @brief Set active Application
     * 
     * @param app pointer to the active Application
     */
    void (*setApp)(App *app);

    /**
     * @brief Renders active Application
     * 
     * @param forced true if forced render, false otherwise
     */
    void (*renderApp)(bool forced);  

    /**
    * @brief Finds the first component which contains the point
    * 
    * @param x x coordinate of the point
    * @param y y coordinate of the point
    * @return pointer to the Component or NULL otherwise
    */
    Component* (*find)(signed short x, signed short y);
    /**
     * @brief Sends onTouch event to the given Component and renders application after 
     * 
     * @param component point to the Component
     * @param x x coordinate of the event 
     * @param y y coordinate of the event 
     * @param tickCount system tick count value
     */
    void (*onTouch)(Component *component, signed short x, signed short y, unsigned long tickCount); 

    /**
     * @brief Sends onMove event to the given Component and renders application after 
     * 
     * @param component point to the Component
     * @param x x coordinate of the event 
     * @param y y coordinate of the event 
     * @param tickCount system tick count value
     */    
    void (*onMove)(Component *component, signed short x, signed short y, unsigned long tickCount); 
    
    /**
     * @brief Sends onRelease event to the given Component and renders application after 
     * 
     * @param component point to the Component
     * @param x x coordinate of the event 
     * @param y y coordinate of the event 
     * @param tickCount system tick count value
     */
    void (*onRelease)(Component *component, signed short x, signed short y, unsigned long tickCount); 

    /**
     * @brief Sends onGesture event to the active Tile and renders application after 
     * 
     * @param component point to the Component
     * @param gesture gesture code
     */
    void (*onGesture)(Gesture gesture);

    /**
     * @brief Sends onButton event to the active Tile and renders application after     
     */
    void (*onButton)();

    /**
     * @brief Sends tick event to the active Tile and renders application after
     * 
     * @param tick current FreeRTOS tick count
     */
    void (*onTick)(unsigned long tick);
} Tiler;

/**
 * @brief Create a default implementation of Tiler and renders application after 
 * 
 * @param tftApi pointer to TFT API
 * @return Tiler new instance
 */
Tiler createTiler(TftApi *tftApi);