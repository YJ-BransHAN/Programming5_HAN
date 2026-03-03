#ifndef MyBMP280_h
#define MyBMP280_h

#include <stdint.h>

#ifdef ARDUINO
#include <Wire.h>
#else
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#endif

#define BMP280_ADDR 0x76

#define REG_PRESS_MSB 0xF7
#define REG_PRESS_LSB 0xF8
#define REG_PRESS_XLSB 0xF9
#define REG_TEMP_MSB 0xFA
#define REG_TEMP_LSB 0xFB
#define REG_TEMP_XLSB 0xFC

#define REG_CALIB_START 0x88

class MyBMP280 {
  public:
    MyBMP280();
    bool begin();

#ifndef ARDUINO
    bool begin(const char* i2cDevice); // Linux version
#endif

    double readTemperature();
    double readPressure();

  private:
    void CalibrationRead();
    float readTemperatureRaw();
    float readPressureRaw();

    double TemperatureConvertion(uint32_t adc_T);
    double PressureConvertion(uint32_t adc_P);

#ifndef ARDUINO
    int file;   // Linux file descriptor
#endif

    uint16_t dig_T1;
    int16_t dig_T2, dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

    uint32_t t_fine;
};

#endif