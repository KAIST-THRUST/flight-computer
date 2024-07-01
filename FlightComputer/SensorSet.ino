#include "SensorSet.h"

SensorSet::SensorSet() {}

void SensorSet::addSensor(Sensor *sensor, enum SensorType sensor_type) {
  sensors[sensor_type] = sensor;
}

void SensorSet::beginAll() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    sensors[i]->begin();
  }
}

void SensorSet::updateAll() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    sensors[i]->update();
  }
}
