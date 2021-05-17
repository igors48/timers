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
    watch->setBrightness(8);

    // Turn off unused power
    watch->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
    watch->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF); // audio device
    watch->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);
}

void showBattState()
{
    uint8_t per;
    float vbus_v, vbus_c, batt_v;
    vbus_v = watch->power->getVbusVoltage();
    vbus_c = watch->power->getVbusCurrent();
    batt_v = watch->power->getBattVoltage();
    per = watch->power->getBattPercentage();
    watch->tft->setCursor(0, 180);
    watch->tft->setTextSize(1);
    watch->tft->setTextFont(1);
    //watch->tft->setTextColor(TFT_GREEN);
    if (watch->power->isVBUSPlug())
    {
        watch->tft->print("P ");
    }
    else
    {
        watch->tft->print("U ");
    }
    watch->tft->setCursor(0, 190);
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
    watch->tft->setCursor(0, 200);
    watch->tft->print("BV: ");
    watch->tft->print(vbus_v);
    watch->tft->print(" mV ");
    watch->tft->setCursor(0, 210);
    watch->tft->print("BC: ");
    watch->tft->print(vbus_c);
    watch->tft->print(" mA ");
    watch->tft->setCursor(0, 220);
    watch->tft->print("Vbat: ");
    watch->tft->print(batt_v);
    watch->tft->print(" mV ");
    watch->tft->setCursor(0, 230);
    //watch->tft->print("Percent: ");
    watch->tft->print(per);
    watch->tft->print(" %");
}

void loop()
{
    showBattState();
    delay(1000);
}