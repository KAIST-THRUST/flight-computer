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
  virtual void begin(float *ptr) = 0; // Begin method.
  virtual void update() = 0;   // Update the sensor_data attribute.
  virtual ~Sensor() = default; // Default destructor.
  virtual String toString() const = 0; // Convert sensor data to string.

  const SensorData &getSensorData() const { // Getter for sensor_data.
    return sensor_data;
  }

protected:
  SensorData sensor_data; // Attribute holding sensor readings.
  float *data_ptr;        // Pointer to sensor_data.values.
};

class AverageSensorValue {
public:
  AverageSensorValue() : avg(0.0), count(0) {}

  void addValue(float value) {
    /* Calculate the new average. */
    avg = (avg * count / (count + 1)) + value / (count + 1);
    count++;
  }

  float getAverage() const { return avg; }

  void reset() {
    avg = 0;
    count = 0;
  }

private:
  float avg;
  int count;
};

#endif
