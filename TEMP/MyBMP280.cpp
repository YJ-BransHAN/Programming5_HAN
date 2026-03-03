#include "MyBMP280.h"

MyBMP280::MyBMP280() {}

#ifdef ARDUINO

bool MyBMP280::begin() {
    Wire.begin();

    Wire.beginTransmission(BMP280_ADDR);
    if (Wire.endTransmission() != 0) return false;

    CalibrationRead();
    return true;
}

#else

bool MyBMP280::begin(const char* device) {

    file = open(device, O_RDWR);

    if (file < 0) {
        perror("Failed to open I2C bus");
        return false;
    }

    if (ioctl(file, I2C_SLAVE, BMP280_ADDR) < 0) {
        perror("Failed to connect to sensor");
        return false;
    }

    CalibrationRead();
    return true;
}

#endif

void MyBMP280::CalibrationRead() {

#ifdef ARDUINO

    Wire.beginTransmission(BMP280_ADDR);
    Wire.write(REG_CALIB_START);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDR, 24);

    dig_T1 = (Wire.read() | (Wire.read() << 8));
    dig_T2 = (Wire.read() | (Wire.read() << 8));
    dig_T3 = (Wire.read() | (Wire.read() << 8));
    dig_P1 = (Wire.read() | (Wire.read() << 8));
    dig_P2 = (Wire.read() | (Wire.read() << 8));
    dig_P3 = (Wire.read() | (Wire.read() << 8));
    dig_P4 = (Wire.read() | (Wire.read() << 8));
    dig_P5 = (Wire.read() | (Wire.read() << 8));
    dig_P6 = (Wire.read() | (Wire.read() << 8));
    dig_P7 = (Wire.read() | (Wire.read() << 8));
    dig_P8 = (Wire.read() | (Wire.read() << 8));
    dig_P9 = (Wire.read() | (Wire.read() << 8));

#else

    uint8_t reg = REG_CALIB_START;
    write(file, &reg, 1);

    uint8_t data[24];
    read(file, data, 24);

    dig_T1 = (data[1] << 8) | data[0];
    dig_T2 = (data[3] << 8) | data[2];
    dig_T3 = (data[5] << 8) | data[4];
    dig_P1 = (data[7] << 8) | data[6];
    dig_P2 = (data[9] << 8) | data[8];
    dig_P3 = (data[11] << 8) | data[10];
    dig_P4 = (data[13] << 8) | data[12];
    dig_P5 = (data[15] << 8) | data[14];
    dig_P6 = (data[17] << 8) | data[16];
    dig_P7 = (data[19] << 8) | data[18];
    dig_P8 = (data[21] << 8) | data[20];
    dig_P9 = (data[23] << 8) | data[22];

#endif
}

float MyBMP280::readTemperatureRaw() {

#ifdef ARDUINO

  Wire.beginTransmission(BMP280_ADDR);
  Wire.write(REG_TEMP_MSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDR, 3);

  uint32_t msb = Wire.read();
  uint32_t lsb = Wire.read();
  uint32_t xlsb = Wire.read();

#else

  uint8_t reg = REG_TEMP_MSB;
  write(file, &reg, 1);

  uint8_t data[3];
  read(file, data, 3);

  uint32_t msb = data[0];
  uint32_t lsb = data[1];
  uint32_t xlsb = data[2];

#endif

  return (msb << 12) | (lsb << 4) | (xlsb >> 4);
}

float MyBMP280::readPressureRaw() {

#ifdef ARDUINO

  Wire.beginTransmission(BMP280_ADDR);
  Wire.write(REG_PRESS_MSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDR, 3);

  uint32_t msb = Wire.read();
  uint32_t lsb = Wire.read();
  uint32_t xlsb = Wire.read();

#else

  uint8_t reg = REG_PRESS_MSB;
  write(file, &reg, 1);

  uint8_t data[3];
  read(file, data, 3);

  uint32_t msb = data[0];
  uint32_t lsb = data[1];
  uint32_t xlsb = data[2];

#endif

  return (msb << 12) | (lsb << 4) | (xlsb >> 4);
}

double MyBMP280::TemperatureConvertion(uint32_t adc_T)
{
double var1, var2, T;
var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) *
(((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
t_fine = (uint32_t)(var1 + var2);
T = (var1 + var2) / 5120.0;
return T;
}

double MyBMP280::PressureConvertion(uint32_t adc_P)
{
double var1, var2, p;
var1 = ((double)t_fine/2.0) - 64000.0;
var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
var2 = var2 + var1 * ((double)dig_P5) * 2.0;
var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
if (var1 == 0.0) return 0;
p = 1048576.0 - (double)adc_P;
p = (p - (var2 / 4096.0)) * 6250.0 / var1;
var1 = ((double)dig_P9) * p * p / 2147483648.0;
var2 = p * ((double)dig_P8) / 32768.0;
p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
return p;
}

double MyBMP280::readTemperature(){
  return TemperatureConvertion((uint32_t)readTemperatureRaw());
}

double MyBMP280::readPressure(){
  return PressureConvertion((uint32_t)readPressureRaw());
}