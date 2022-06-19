/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Group of components definition
 * 
 * Group it is the list of child components  
 * Group renders itself by rendering the children  
*/
#pragma once

#include "component.hpp"

/**
 * @brief group child components list
 */
typedef struct
{
    unsigned char childrenCount;
    void **children; 
    void (*onTick)();
} GroupState;

/**
 * @brief Renders a group
 * 
 * Calls render for each child in the list
 * 
 * @param group pointer to a group 
 * @param forced render mode
 * @param tftApi pointer to TFT API 
 */
void groupRender(Component *group, bool forced, TftApi *tftApi);

Component* groupContains(Component *group, signed short x, signed short y);

void groupMount(Component *group, signed short x, signed short y);

/**
 * @brief Check if group state is modified
 * 
 * In case of group always return true
 * 
 * @param group pointer to a group 
 * @return true always 
 * @return false never
 */
bool groupIsStateModified(Component *group);

void groupUpdateState(Component *group);

void groupNoopTick();