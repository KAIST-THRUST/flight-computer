/**
 * @file SensorSet.h
 * @brief A sensor set class that manages multiple sensors.
 */

#ifndef SENSORSET_H
#define SENSORSET_H

#include "BarometerSensor.h"
#include "GPSSensor.h"
#include "IMUSensor.h"
#include "Sensor.h"
#include "VoltmeterSensor.h"
#include "config.h"

class SensorSet {
public:
  enum SensorType {
    GPS,
    IMU,
    BAROMETER,
    VOLTMETER,
    PRESSURE,
    SENSOR_COUNT
  };
  SensorSet();
  /* Add a sensor to the set. */
  void addSensor(Sensor *sensor, enum SensorType sensor_type);
  void addSensor(GPSSensor *sensor);
  void addSensor(IMUSensor *sensor);
  void addSensor(BarometerSensor *sensor);
  void addSensor(VoltmeterSensor *sensor);

  void beginAll();  // Begin all sensors.
  void updateAll(); // Update all sensors.

  /* Getter for specific sensor data. */
  float getValue(enum GPSSensor::GPSDataIndex index) const;
  float getValue(enum IMUSensor::IMUDataIndex index) const;
  float getValue(enum BarometerSensor::BarometerDataIndex index) const;
  float getValue(enum VoltmeterSensor::VoltmeterDataIndex index) const;

private:
  Sensor *sensors[SENSOR_COUNT];
};

#endif
