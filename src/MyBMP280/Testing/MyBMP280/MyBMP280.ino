#include <MyBMP280.h> // Include your header

MyBMP280 sensor;

void setup() {
  Serial.begin(9600);
  sensor.begin();
}

void loop() {
  float temp = sensor.readTemperature();
  Serial.print(temp);
  Serial.println(" C");
  delay(2000);
}