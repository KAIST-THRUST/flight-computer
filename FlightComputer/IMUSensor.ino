#include "IMUSensor.h"

IMUSensor::IMUSensor() {
  bno = Adafruit_BNO055(55, 0x28, &Wire);
  sensorData.data_count = DATA_COUNT;
}

void IMUSensor::begin() {
  if (!bno.begin()) {
    printErrorMessageToSerial(
        "no BNO055 detected ... Check your wiring or I2C ADDR!");
  }
}

void IMUSensor::update() {}

String IMUSensor::toString() {
  return "IMU Sensor";
}
