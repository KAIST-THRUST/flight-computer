/**
 * @file SensorSet.h
 * @brief A sensor set class that manages multiple sensors.
 */

#ifndef SENSORSET_H
#define SENSORSET_H

#include "Sensor.h"

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
  // Add a sensor to the set.
  void addSensor(Sensor *sensor, enum SensorType sensor_type);
  void beginAll();  // Begin all sensors.
  void updateAll(); // Update all sensors.

private:
  Sensor *sensors[SENSOR_COUNT];
};

#endif
