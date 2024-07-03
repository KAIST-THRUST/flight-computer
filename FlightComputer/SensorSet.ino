#include "SensorSet.h"

SensorSet::SensorSet() {}

void SensorSet::addSensor(Sensor *sensor, enum SensorType sensor_type) {
  sensors[sensor_type] = sensor;
}

void SensorSet::addSensor(GPSSensor *sensor) { sensors[GPS] = sensor; }

void SensorSet::addSensor(IMUSensor *sensor) { sensors[IMU] = sensor; }

void SensorSet::addSensor(BarometerSensor *sensor) {
  sensors[BAROMETER] = sensor;
}

void SensorSet::addSensor(VoltmeterSensor *sensor) {
  sensors[VOLTMETER] = sensor;
}

void SensorSet::beginAll() {
  /* Begin all sensors. */
  for (Sensor *sensor : sensors) {
    sensor->begin();
  }
}

void SensorSet::updateAll() {
  /* Update all sensors. */
  for (Sensor *sensor : sensors) {
    sensor->update();
  }
}

float SensorSet::getValue(enum GPSSensor::GPSDataIndex index) const {
  return sensors[GPS]->getSensorData().values[index];
}

float SensorSet::getValue(enum IMUSensor::IMUDataIndex index) const {
  return sensors[IMU]->getSensorData().values[index];
}

float SensorSet::getValue(
    enum BarometerSensor::BarometerDataIndex index) const {
  return sensors[BAROMETER]->getSensorData().values[index];
}

float SensorSet::getValue(
    enum VoltmeterSensor::VoltmeterDataIndex index) const {
  return sensors[VOLTMETER]->getSensorData().values[index];
}
