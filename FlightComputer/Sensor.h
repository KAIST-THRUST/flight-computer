/**
 * @file Sensor.h
 * @brief Basic class structure for sensors.
 */

#ifndef SENSOR_H
#define SENSOR_H

struct SensorData {
  float values[10]; // Array of sensor output values.
  int data_count;   // Number of output data points.
};

class Sensor {
public:
  virtual void begin() = 0;    // Begin method. Place it in setup().
  virtual void Update() = 0;   // Update the sensorData attribute.
  virtual ~Sensor() = default; // Default destructor.

  const SensorData &getSensorData() const { // Getter for sensorData.
    return sensorData;
  }

protected:
  SensorData sensorData; // Attribute holding sensor readings.
};

#endif
