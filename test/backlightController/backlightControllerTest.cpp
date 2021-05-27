#include <unity.h>
#include <Arduino.h>
#include "backlightController.hpp"

void setup() {
    UNITY_BEGIN();
    BackligthControllerParameters p = {};
    backlightController(&p);
    UNITY_END();
}

void loop() {

}