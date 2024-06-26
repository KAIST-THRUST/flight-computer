#include "SensorSet.h"

SensorSet::SensorSet() : sensor_count(0) {}

void SensorSet::addSensor(Sensor *sensor) {
  sensors[sensor_count++] = sensor;
}

void SensorSet::beginAll() {
  for (int i = 0; i < sensor_count; i++) {
    sensors[i]->begin();
  }
}

void SensorSet::updateAll() {
  for (int i = 0; i < sensor_count; i++) {
    sensors[i]->update();
  }
}
