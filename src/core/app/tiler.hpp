/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief application's tile controller
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
    * @copydoc Component.contains !!! <- fix the description
    */
    Component* (*contains)(signed short x, signed short y); // todo rename to find
    void (*onTouch)(Component *component, signed short x, signed short y, unsigned long tickCount); 
    void (*onMove)(Component *component, signed short x, signed short y, unsigned long tickCount); 
    void (*onRelease)(Component *component, signed short x, signed short y, unsigned long tickCount); 
    void (*onGesture)(Gesture gesture); 
    void (*onButton)();
} Tiler;

/**
 * @brief Create a default implementation of Tiler
 * 
 * @param tftApi pointer to TFT API
 * @return Tiler new instance
 */
Tiler createTiler(TftApi *tftApi);