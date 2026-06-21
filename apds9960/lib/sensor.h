/*
* Calico Rose
* June 20, 2026
* Sensor setup / header for general sensor use / initialization.
*/

#include <Arduino.h>
#include <Wire.h>
#include "APDS9960-SOLDERED.h"

#define APDS9960_I2C_ADDR  0x39
#define GCONF4             0xAB
#define GFIFO_CLR_BIT      0x04
#define APDS9960_I2C_ADDR  0x39

APDS_9960 APDS;

void clearGestureFIFO() {
    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(GCONF4);
    Wire.write(GFIFO_CLR_BIT);  // Set GFIFO_CLR bit
    Wire.endTransmission();
}

#define APDS9960_I2C_ADDR 0x39

uint8_t readReg(uint8_t reg) {
    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(APDS9960_I2C_ADDR, 1);
    return Wire.available() ? Wire.read() : 0xFF;
}

void dumpGestureRegisters() {
    Serial.println("=== APDS-9960 Gesture Register Dump ===");
    Serial.print("ENABLE   (0x80): 0x"); Serial.println(readReg(0x80), HEX);
    Serial.print("GCONF1   (0xA2): 0x"); Serial.println(readReg(0xA2), HEX);
    Serial.print("GCONF2   (0xA3): 0x"); Serial.println(readReg(0xA3), HEX);
    Serial.print("GCONF3   (0xAA): 0x"); Serial.println(readReg(0xAA), HEX);
    Serial.print("GCONF4   (0xAB): 0x"); Serial.println(readReg(0xAB), HEX);
    Serial.print("GPENTH   (0xA0): 0x"); Serial.println(readReg(0xA0), HEX);
    Serial.print("GEXTH    (0xA1): 0x"); Serial.println(readReg(0xA1), HEX);
    Serial.print("GFLVL    (0xAE): 0x"); Serial.println(readReg(0xAE), HEX);
    Serial.print("GSTATUS  (0xAF): 0x"); Serial.println(readReg(0xAF), HEX);
    Serial.print("PPULSE   (0x8E): 0x"); Serial.println(readReg(0x8E), HEX);
    Serial.print("CONTROL  (0x8F): 0x"); Serial.println(readReg(0x8F), HEX);
    Serial.println("========================================");
}

#define APDS9960_I2C_ADDR 0x39

void writeReg(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

void fixGestureEngine() {
    // Step 1: Force exit gesture mode by clearing GMODE bit
    // Keep GIEN (bit 1) but clear GMODE (bit 0)
    writeReg(0xAB, 0x02);  // GCONF4: GIEN=1, GMODE=0

    delay(10);

    // Step 2: Disable gesture engine entirely while we reset
    // ENABLE 0x49 = 0b01001001, clear bit 6 to disable gesture
    writeReg(0x80, 0x09);  // PON + PEN, no GEN

    delay(10);

    // Step 3: Clear the FIFO
    writeReg(0xAB, 0x06);  // GCONF4: GFIFO_CLR (bit 2) + GIEN (bit 1)

    delay(10);

    // Step 4: Set sane thresholds so noise doesn't flood the FIFO again
    writeReg(0xA0, 0x28);  // GPENTH: enter gesture mode at proximity ~40
    writeReg(0xA1, 0x1E);  // GEXTH:  exit gesture mode at proximity ~30

    delay(10);

    // Step 5: Re-enable gesture engine
    writeReg(0x80, 0x49);  // Restore ENABLE: PON + PEN + GEN

    delay(50);

    // Verify
    Serial.print("GCONF4 after fix  (0xAB): 0x"); Serial.println(readReg(0xAB), HEX);
    Serial.print("GFLVL after fix   (0xAE): 0x"); Serial.println(readReg(0xAE), HEX);
    Serial.print("GSTATUS after fix (0xAF): 0x"); Serial.println(readReg(0xAF), HEX);
    Serial.print("GPENTH after fix  (0xA0): 0x"); Serial.println(readReg(0xA0), HEX);
    Serial.print("GEXTH after fix   (0xA1): 0x"); Serial.println(readReg(0xA1), HEX);
}

void sensorSetup() {
    Wire.begin();
    Serial.begin(115200);
    while (!Serial);
    if (!APDS.begin()) {
        Serial.println("Error initializing APDS-9960 sensor!");
        while(1);
    }
    APDS.setGestureSensitivity(70);
    
    fixGestureEngine();  // re-enable this
    
    // Clear PEN (proximity enable) from ENABLE register
    // This stops gestureAvailable() from calling setGestureMode(false) every cycle
    writeReg(0x80, 0x41);  // PON + GEN only, PEN = 0
    
    delay(100);
    
    // Verify
    Serial.print("ENABLE after setup: 0x");
    Serial.println(readReg(0x80), HEX);
}