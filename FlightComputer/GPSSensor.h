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
    LATITUDE,        // in degrees/minutes
    LATITUDE_LS,     // in degrees/minutes
    LONGITUDE,       // in degrees/minutes
    LONGITUDE_LS,    // in degrees/minutes
    ALTITUDE,        // in meters
    ALTITUDE_LS,     // in meters
    GEOID_HEIGHT,    // in meters
    GEOID_HEIGHT_LS, // in meters
    DATA_COUNT
  };
  GPSSensor();
  void begin(float *ptr) override;
  void update() override;
  String toString() const override;

  float getLatitude() const;
  float getLongitude() const;
  float getAltitude() const;

private:
  Adafruit_GPS GPS;
  AverageSensorValue latitude_ls;
  AverageSensorValue longitude_ls;
  AverageSensorValue altitude_ls;
  AverageSensorValue geoid_height_ls;
};

#endif
