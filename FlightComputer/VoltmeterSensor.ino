#include "VoltmeterSensor.h"

VoltmeterSensor::VoltmeterSensor() {
  sensorData.data_count = DATA_COUNT;
}

void VoltmeterSensor::begin() {
  if (!ads.begin()) {
    printErrorMessageToSerial("ADS1115 not found!");
  }
}

void VoltmeterSensor::update() {}

String VoltmeterSensor::toString() const { return "[Voltmeter]"; }
