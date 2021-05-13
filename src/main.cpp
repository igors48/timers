#include <Arduino.h>
// Select you T-Watch in the platformio.ini file 
#include <LilyGoWatch.h>

// C++ object which will allow access to the functions of the Watch 
TTGOClass *watch;

void setup() {
    // Get Watch object and set up the display 
    watch = TTGOClass::getWatch();
    watch->begin();     
    watch->openBL();
    
    // Use SPI_eTFT library to display text on screen 
    watch->tft->fillScreen(TFT_BLACK);
    watch->tft->setTextFont(2);
    watch->tft->setTextSize(1);
    watch->tft->setTextColor(TFT_GREEN);
    watch->tft->setCursor(0, 0);
    watch->tft->println(F("Hello T-World (battery percentage)"));
}

void loop() {}