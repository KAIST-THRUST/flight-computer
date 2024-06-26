/**
 * @file Sensor.h
 * @brief Basic class structure for sensors.
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"

struct SensorData {
  float values[MAX_DATA_COUNT] = {}; // Array of sensor output values.
  int data_count;                    // Number of output data points.
};

class Sensor {
public:
  virtual void begin() = 0;      // Begin method. Place it in setup().
  virtual void update() = 0;     // Update the sensorData attribute.
  virtual ~Sensor() = default;   // Default destructor.
  virtual String toString() = 0; // Convert sensor data to string.

  const SensorData &getSensorData() const { // Getter for sensorData.
    return sensorData;
  }

protected:
  SensorData sensorData; // Attribute holding sensor readings.
};

#endif
