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
    LATITUDE,        // in degrees
    LONGITUDE,       // in degrees
    ALTITUDE,        // in meters
    GEOID_HEIGHT,    // in meters
    LATITUDE_LS,     // in degrees
    LONGITUDE_LS,    // in degrees
    ALTITUDE_LS,     // in meters
    GEOID_HEIGHT_LS, // in meters
    IS_UPDATED,      // 0 or 1
    DATA_COUNT
  };
  GPSSensor();
  void begin(float *ptr) override;
  void update() override;
  String toString() const override;

  bool isValid() const;
  float getLatitude() const;
  float getLongitude() const;
  float getAltitude() const;

private:
  Adafruit_GPS GPS;
  bool is_fixed;
};

#endif
