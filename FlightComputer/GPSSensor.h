/**
 * @file GPSSensor.h
 * @brief A basic GPS class that reads data from adafruit ultimate GPS
 * Sensor.
 */

#ifndef GPSSENSOR_H
#define GPSSENSOR_H

#include "Sensor.h"
#include "config.h"
#include "util.h"
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>

#define GPSECHO false

class GPSSensor : public Sensor {
public:
  enum GPSDataIndex {
    LATITUDE,     // in degrees/minutes
    LONGITUDE,    // in degrees/minutes
    ALTITUDE,     // in meters
    ALTITUDE_AVG, // in meters
    SPEED,        // in m/s
    DATA_COUNT
  };
  GPSSensor();
  void begin() override;
  void update() override;
  String toString() const override;

  float getLatitude() const;
  float getLongitude() const;
  float getAltitude() const;

private:
  Adafruit_GPS GPS;
  AverageSensorValue altitude_avg;
};

#endif
