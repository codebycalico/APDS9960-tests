/*
 * Calico Rose
 * June 20, 2026
 * Color and gesture sensor testing - APDS-9960 by Soldered
 * Custom register polling from Claude to try and enable proximity & gesture "simultaneously.""
 */

#include <Arduino.h>
#include <Wire.h>

#define APDS9960_I2C_ADDR 0x39

uint8_t readReg(uint8_t reg)
{
    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(APDS9960_I2C_ADDR, 1);
    return Wire.available() ? Wire.read() : 0xFF;
}

void writeReg(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

String detectGestureRaw() {
    writeReg(0x80, 0x4D);

    uint8_t gstatus = readReg(0xAF);
    uint8_t flvl    = readReg(0xAE);

    if (flvl < 4 || !(gstatus & 0x01)) return "";  // need at least 4 datasets for reliable trajectory

    // Read all datasets into arrays
    uint8_t u_data[32], d_data[32], l_data[32], r_data[32];
    uint8_t count = min(flvl, (uint8_t)32);

    for (uint8_t i = 0; i < count; i++) {
        Wire.beginTransmission(APDS9960_I2C_ADDR);
        Wire.write(0xFC);
        Wire.endTransmission();
        Wire.requestFrom(APDS9960_I2C_ADDR, 4);
        u_data[i] = Wire.read();
        d_data[i] = Wire.read();
        l_data[i] = Wire.read();
        r_data[i] = Wire.read();
    }

    writeReg(0xAB, 0x06);  // clear FIFO

    // Average first quarter and last quarter of datasets
    uint8_t first = max((uint8_t)1, (uint8_t)(count / 4));
    uint8_t last  = max((uint8_t)1, (uint8_t)(count / 4));

    int u_first = 0, d_first = 0, l_first = 0, r_first = 0;
    int u_last  = 0, d_last  = 0, l_last  = 0, r_last  = 0;

    for (uint8_t i = 0; i < first; i++) {
        u_first += u_data[i];
        d_first += d_data[i];
        l_first += l_data[i];
        r_first += r_data[i];
    }
    for (uint8_t i = count - last; i < count; i++) {
        u_last += u_data[i];
        d_last += d_data[i];
        l_last += l_data[i];
        r_last += r_data[i];
    }

    u_first /= first; d_first /= first;
    l_first /= first; r_first /= first;
    u_last  /= last;  d_last  /= last;
    l_last  /= last;  r_last  /= last;

    // Trajectory deltas - how each photodiode changed from start to end
    int ud_delta = (u_last - u_first) - (d_last - d_first);
    int lr_delta = (l_last - l_first) - (r_last - r_first);

    Serial.print("ud_delta="); Serial.print(ud_delta);
    Serial.print(" lr_delta="); Serial.println(lr_delta);

    // Minimum signal
    if (abs(ud_delta) < 30 && abs(lr_delta) < 30) return "";

    // Dominance check
    if (abs(ud_delta) > abs(lr_delta) * 1.5) {
        return (ud_delta > 0) ? "UP" : "DOWN";
    } else if (abs(lr_delta) > abs(ud_delta) * 1.5) {
        return (lr_delta > 0) ? "LEFT" : "RIGHT";
    }

    return "";
}

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    while (!Serial)
        ;

    // Power on and configure sensor
    writeReg(0x80, 0x00); // disable everything first
    delay(10);

    // LED drive & gain
    writeReg(0x8E, 0x8F); // PPULSE: 16 pulses, 8us
    writeReg(0x8F, 0x01); // CONTROL: LED drive 100mA, proximity gain x2

    // Gesture config
    writeReg(0xA0, 0x28); // GPENTH: enter gesture at proximity ~40
    writeReg(0xA1, 0x1E); // GEXTH:  exit gesture at proximity ~30
    writeReg(0xA2, 0x41); // GCONF1: 4 datasets to generate interrupt, FIFO threshold
    writeReg(0xA3, 0x42); // GCONF2: gain x4, LED drive 100mA, wait 2.78ms
    writeReg(0xAA, 0x00); // GCONF3: all photodiodes active

    // Clear any stale FIFO state
    writeReg(0xAB, 0x06); // GCONF4: GFIFO_CLR
    delay(10);

    // Enable: PON + WEN + PEN + GEN
    writeReg(0x80, 0x4D);
    delay(100);

    Serial.println("Sensor initialized.");
}

void loop()
{
    String gesture = detectGestureRaw();
    if (gesture != "") { Serial.println(gesture); }

    delay(50);
}