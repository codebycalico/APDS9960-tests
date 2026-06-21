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

void debugGesture() {
     // Read proximity through the library first
    int prox = -1;
    if (APDS.proximityAvailable()) {
        prox = APDS.readProximity();
    }

    uint8_t pdata   = readReg(0x9C);   // PDATA
    uint8_t enable  = readReg(0x80);   // ENABLE
    uint8_t control = readReg(0x8F);   // CONTROL - LED drive
    uint8_t gconf4  = readReg(0xAB);   // GCONF4

    Serial.print("LibProx: ");   Serial.print(prox);
    Serial.print(" | PDATA: ");  Serial.print(pdata);
    Serial.print(" | ENABLE: 0x"); Serial.print(enable, HEX);
    Serial.print(" | CONTROL: 0x"); Serial.print(control, HEX);
    Serial.print(" | GCONF4: 0x"); Serial.println(gconf4, HEX);
}

void detectGesture() {
    if (APDS.gestureAvailable()) {
        // a gesture was detected, read and print to Serial Monitor
        int gesture = APDS.readGesture();

        switch (gesture) {  //Determine which gesture was captured
        case GESTURE_UP:
            Serial.println("UP");
            break;

        case GESTURE_DOWN:
            Serial.println("DOWN");
            break;

        case GESTURE_LEFT:
            Serial.println("LEFT");
            break;

        case GESTURE_RIGHT:
            Serial.println("RIGHT");
            break;

        default:
            // ignore
            Serial.println("NONE");
            break;
        }
    } else {
        Serial.println("Gesture detection unavailable.");
    }
}

/*
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

void detectAll() {
    if(!APDS.gestureAvailable()) {
        Serial.println("Gesture detection unavailable.");
        return;
    }

    if(!APDS.proximityAvailable()) {
        Serial.println("Proximity detection unavailable.");
        return;
    }
 
    // a gesture was detected, read and print to Serial Monitor
    int gesture = APDS.readGesture();
    int proximity = APDS.readProximity();

    switch (gesture) {  //Determine which gesture was captured
        case GESTURE_UP:
            Serial.println("1 0 0 0 " + proximity);
            break;

        case GESTURE_DOWN:
            Serial.println("0 1 0 0 " + proximity);
            break;

        case GESTURE_LEFT:
            Serial.println("0 0 1 0 " + proximity);
            break;

        case GESTURE_RIGHT:
            Serial.println("0 0 0 1 " + proximity);
            break;

        default:
            // ignore
            Serial.println("0 0 0 0 " + proximity);
            break;
    }
} */