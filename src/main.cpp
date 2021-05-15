#include <Arduino.h>
// Select you T-Watch in the platformio.ini file
#include <LilyGoWatch.h>

// C++ object which will allow access to the functions of the Watch
TTGOClass *watch;

void setup()
{
    // Get Watch object and set up the display
    watch = TTGOClass::getWatch();
    watch->begin();
    watch->openBL();

    // Use SPI_eTFT library to display text on screen
    // watch->tft->fillScreen(TFT_BLACK);
    // watch->tft->setTextFont(2);
    // watch->tft->setTextSize(1);
    // watch->tft->setTextColor(TFT_GREEN);
    // watch->tft->setCursor(0, 0);
    // watch->tft->println(F("Hello T-World (battery percentage)"));
    //watch->power->

    uint8_t per;
    float vbus_v, vbus_c, batt_v;
    vbus_v = watch->power->getVbusVoltage();
    vbus_c = watch->power->getVbusCurrent();
    batt_v = watch->power->getBattVoltage();
    per = watch->power->getBattPercentage();
    watch->tft->setCursor(0, 230);
    if (watch->power->isVBUSPlug())
    {
        watch->tft->print("P ");
    }
    else
    {
        watch->tft->print("U ");
    }
    //watch->tft->setCursor(0, 70);
    if (watch->power->isChargeing())
    {
        watch->tft->print("C: ");
        watch->tft->print(watch->power->getBattChargeCurrent());
    }
    else
    {
        // Show current consumption
        watch->tft->print("D: ");
        watch->tft->print(watch->power->getBattDischargeCurrent());
    }
    watch->tft->print(" mA ");

    // Print the values
    //watch->tft->setCursor(0, 100);
    watch->tft->print("BV: ");
    watch->tft->print(vbus_v);
    watch->tft->print(" mV ");
    //watch->tft->setCursor(0, 130);
    watch->tft->print("BC: ");
    watch->tft->print(vbus_c);
    watch->tft->print(" mA ");
    //watch->tft->setCursor(0, 160);
    watch->tft->print("Vbat: ");
    watch->tft->print(batt_v);
    watch->tft->print(" mV ");
    //watch->tft->setCursor(0, 190);
    //watch->tft->print("Percent: ");
    watch->tft->print(per);
    watch->tft->print(" %");
}

void loop() {}