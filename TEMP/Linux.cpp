#include <iostream>
#include <unistd.h>
#include "MyBMP280.h"

int main() {

    MyBMP280 sensor;

    if (!sensor.begin("/dev/i2c-1")) {
        std::cout << "Sensor not found" << std::endl;
        return 1;
    }

    while(true)
    {
        double t = sensor.readTemperature();
        double p = sensor.readPressure();

        std::cout << "Temp = " << t << std::endl;
        std::cout << "Pressure = " << p << std::endl;

        sleep(2);
    }

    return 0;
}