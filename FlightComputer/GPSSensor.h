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

class GPSSensor : public Sensor {
public:
  enum GPSDataIndex { LATITUDE, LONGITUDE, ALTITUDE, DATA_COUNT };
  GPSSensor();
  void begin() override;
  void update() override;
  String toString() const override;

  float getLatitude() const;
  float getLongitude() const;
  float getAltitude() const;

private:
  Adafruit_GPS GPS;
};

#endif
