/*
* Calico Rose
* June 20, 2026
* Sensor setup / header for general sensor use / initialization.
* Sparkfun library Platformio setup: https://registry.platformio.org/libraries/sparkfun/SparkFun%20APDS9960%20RGB%20and%20Gesture%20Sensor/installation
* Sparkfun library GitHub: https://github.com/sparkfun/SparkFun_APDS-9960_Sensor_Arduino_Library
*/

/*
#include <Arduino.h>
#include <SparkFun_APDS9960.h>

APDS_9960 APDS;

void sensorSetup() {
    Serial.begin(115200); //Begin serial communication with PC
    while (!Serial); //Wait until serial becomes active

    if (!APDS.begin()) { //Begin communication with sensor
      Serial.println("Error initializing APDS-9960 sensor!");
      while(1); //Loop forever if sensor is not available
    }

    // for setGestureSensitivity(..) a value between 1 and 100 is required.
    // Higher values make the gesture recognition more sensitive but less accurate
    // (a wrong gesture may be detected). Lower values makes the gesture recognition
    // more accurate but less sensitive (some gestures may be missed).
    // Default is 80
    //APDS.setGestureSensitivity(70);

    Serial.println("Sensor initialized.");
} */