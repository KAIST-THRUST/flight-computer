#include "util.h"

void updateServoFromSerial(NonBlockingServo &serv) {
  static int degree = 0;
  // Read data from serial if sent
  if (Serial.available()) {
    Serial.readBytes((char *)&degree, 1);
  }
  // Rotate servo if it is not rotating
  if (!serv.isrotating()) {
    serv.rotate(degree);
  }
}

void printErrorMessageToSerial(String s) {
  Serial.print("Error: ");
  Serial.println(s);
}

void printSensorDataToSerial(const Sensor &sensor) {
  Serial.println(sensor.toString());
}
