/*
* Calico Rose
* June 20, 2026
* Color and gesture sensor testing - APDS-9960 by Soldered
* Note: Their GitHub library isn't available on Platformio natively so it must be manually linked in the .ini file.
*/

#include "../lib/sensorReadings.h"

void setup()
{
    sensorSetup();

    Serial.println("Gesture detection started...");
}

void loop() 
{
  detectGesture();
  //detectColor();
  //detectProximity();
}