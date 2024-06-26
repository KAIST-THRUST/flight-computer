/**
 * @file SensorSet.h
 * @brief A sensor set class that manages multiple sensors.
 */

#ifndef SENSORSET_H
#define SENSORSET_H

#include "Sensor.h"

class SensorSet {
public:
  SensorSet();
  void addSensor(Sensor *sensor); // Add a sensor to the set.
  void beginAll();                // Begin all sensors.
  void updateAll();               // Update all sensors.

private:
  Sensor *sensors[N_SENSORS];
  int sensor_count;
};

#endif
