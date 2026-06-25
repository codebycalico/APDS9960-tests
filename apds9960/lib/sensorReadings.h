/*
* Calico Rose
* June 20, 2026
* Gesture, color, and proximity sensor detection header.
* Reference code: 
* https://github.com/SolderedElectronics/Soldered-APDS9960-Light-Gesture-Color-Sensor-Arduino-Library/tree/main/examples
*/

/**
 **************************************************
 *
 * @file        ColorSensor.ino
 * @brief       This example reads gesture data from the on-board APDS-9960 
 *              sensor and prints any detected gestures to the Serial Monitor.
 *
 *              Gesture directions are as follows:
 *              - UP:    from USB connector towards antenna
 *              - DOWN:  from antenna towards USB connector
 *              - LEFT:  from analog pins side towards digital pins side
 *              - RIGHT: from digital pins side towards analog pins side
 *    
 *              Product: https://solde.red/333047
 *
 *              Modified by soldered.com
 ***************************************************/

#include "sensor.h"

float timeout = 5000;
float lastGestureDetection;
bool timedOut = false;

void detectGesture() {
    if (APDS.gestureAvailable()) {
        // a gesture was detected, read and print to Serial Monitor
        int gesture = APDS.readGesture();

        switch (gesture) {  //Determine which gesture was captured
        case GESTURE_UP:
            Serial.println("UP");
            lastGestureDetection = millis();
            timedOut = false;
            break;

        case GESTURE_DOWN:
            Serial.println("DOWN");
            lastGestureDetection = millis();
            timedOut = false;
            break;

        case GESTURE_LEFT:
            Serial.println("LEFT");
            lastGestureDetection = millis();
            timedOut = false;
            break;

        case GESTURE_RIGHT:
            Serial.println("RIGHT");
            lastGestureDetection = millis();
            timedOut = false;
            break;

        default:
            // ignore
            Serial.println("NONE");
            lastGestureDetection = millis();
            timedOut = false;
            break;
        }
    }
    if(millis() - lastGestureDetection > timeout && !timedOut) {
        Serial.println("TIMEOUT");
        timedOut = true;
    }
}


void detectColor() {
     // check if a color reading is available
     // Wait for color reading to be available
    while (!APDS.colorAvailable()) { delay(5); }

    int r, g, b;  //Initialize variables for color intensities

    // read the color
    APDS.readColor(r, g, b);

    // print the values
    Serial.print("r = ");
    Serial.print(r);
    Serial.print("| g = ");
    Serial.print(g);
    Serial.print("| b = ");
    Serial.println(b);
    Serial.println();

    // wait a bit before reading again
    delay(1000);
}

void detectProximity() {
    // check if a proximity reading is available
    if (APDS.proximityAvailable())
    {
        // read the proximity
        // - 0   => close
        // - 255 => far
        // - -1  => error
        int proximity = APDS.readProximity();

        // print value to the Serial Monitor
        Serial.println(proximity);
    }

    // wait a bit before reading again
    delay(100);
}