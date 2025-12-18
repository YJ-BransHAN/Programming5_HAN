#include "MyBMP280.h"
#include <Wire.h>

#define BMP280_ADDRESS 0x76 // Default I2C address

MyBMP280::MyBMP280() {
  // Constructor is empty for now
}

bool MyBMP280::begin() {
  Wire.begin();
  Wire.beginTransmission(BMP280_ADDRESS);
  if (Wire.endTransmission() == 0) {
    // Very simple initialization sequence
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(0xF4); // control register
    Wire.write(0x27); // normal mode, temp and pressure
    Wire.endTransmission();
    return true;
  }
  return false;
}

float MyBMP280::readTemperature() {
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(0xFA); // temp MSB
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 3);

  if (Wire.available() == 3) {
    long adc_T = (long)Wire.read() << 12 | (long)Wire.read() << 4 | (long)Wire.read() >> 4;
    // Simplified calculation, for demonstration only
    return (float)adc_T / 512.0 + 22.5;
  }
  return -999; // Error value if read fails
}

float MyBMP280::readPressure() {
  // Similar logic for pressure (optional, can be extended)
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(0xF7); // pressure MSB
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 3);

  if (Wire.available() == 3) {
    long adc_P = (long)Wire.read() << 12 | (long)Wire.read() << 4 | (long)Wire.read() >> 4;
    // Simplified calculation (for demo only)
    return (float)adc_P / 100.0;
  }
  return -999; // Error value
}