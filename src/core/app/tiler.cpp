/**
 * @file 
 * @author Igor Usenko (github: igors48)
 * @brief Default implementation of the application's tile controller
*/
#include "tiler.hpp"

/**
 * @brief Pointer to the active Application
 */
static App *activeApp;

/**
 * @brief Pointer to the TFT API
 */
static TftApi *tilerTftApi;

/**
 * @brief Mode of rendering
 */
static bool developmentMode;

/**
 * @brief Draws background grid for development mode
 */
static void drawGrid()
{
    for (int i = 0; i <= 240; i += 10)
    {
        (tilerTftApi->drawFastHLine)(0, i, 240, 0x10A2);
        (tilerTftApi->drawFastVLine)(i, 0, 240, 0x10A2); // 20, 20, 20
    }
}

/**
 * @copydoc Tiler.renderApp
 */
static void renderApp(bool forced)
{
    Component *activeTile = (activeApp->getActiveTile)();
    if (forced)
    {
        (tilerTftApi->fillRect)(0, 0, 240, 240, COLOR_BLACK);
        if (developmentMode)
        {
            drawGrid();
        }
    }
    (activeTile->render)(activeTile, forced, tilerTftApi);
    (activeTile->updateState)(activeTile);
}

/**
 * @copydoc Tiler.setApp
 */
static void setApp(App *app)
{
    activeApp = app;
}

/**
 * @copydoc Tiler.contains
 */
static Component *contains(signed short x, signed short y)
{
    Component *activeTile = (activeApp->getActiveTile)();
    return (activeTile->contains)(activeTile, x, y);
}

/**
 * @copydoc Tiler.onTouch
 */
static void onTouch(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    (component->onTouch)(component, x, y, tickCount);
    renderApp(false);
}

/**
 * @copydoc Tiler.onMove
 */
static void onMove(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    (component->onMove)(component, x, y, tickCount);
    renderApp(false);
}

/**
 * @copydoc Tiler.onRelease
 */
static void onRelease(Component *component, signed short x, signed short y, unsigned long tickCount)
{
    (component->onRelease)(component, x, y, tickCount);
    renderApp(false);
}

/**
 * @copydoc Tiler.onGesture
 */
static void onGesture(Gesture gesture)
{
    Component *activeTile = (activeApp->getActiveTile)();
    (activeTile->onGesture)(activeTile, gesture);
    renderApp(false);
}

/**
 * @copydoc Tiler.onButton
 */
static void onButton()
{
    developmentMode = !developmentMode;
    Component *activeTile = (activeApp->getActiveTile)();
    (activeTile->onButton)(activeTile);
    renderApp(true /*false*/);
}

Tiler createTiler(TftApi *tftApi)
{
    tilerTftApi = tftApi;

    developmentMode = false;

    return {
        .setApp = setApp,
        .renderApp = renderApp,
        .contains = contains,
        .onTouch = onTouch,
        .onMove = onMove,
        .onRelease = onRelease,
        .onGesture = onGesture,
        .onButton = onButton,
    };
}
