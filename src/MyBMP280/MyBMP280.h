#ifndef MYBMP280_H
#define MYBMP280_H

#include <Arduino.h>

class MyBMP280 {
  public:
    MyBMP280();
    bool begin();
    float readTemperature();
    float readPressure();
  private:
    // You can add private variables here if needed for calibration
};

#endif