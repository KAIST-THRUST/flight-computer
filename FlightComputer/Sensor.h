/**
 * @file Sensor.h
 * @brief Basic class structure for sensors.
 */

#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
public:
  virtual void begin() = 0;
  virtual float readValue() = 0;
  virtual ~Sensor() = default;
};

#endif
