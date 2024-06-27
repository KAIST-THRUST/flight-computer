/**
 * @file PressureSensor.h
 * @brief A pressure sensor class that reads data from BMP280 sensor.
 */

#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include "Sensor.h"
#include "config.h"
#include "util.h"
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Wire.h>

class PressureSensor : public Sensor {
public:
  enum PressureDataIndex {
    PRESSURE,    // in hPa
    TEMPERATURE, // in Celsius
    ALTITUDE,    // in meters
    DATA_COUNT
  };
  PressureSensor();
  void begin() override;
  void update() override;
  String toString() const override;

private:
  Adafruit_BMP280 bmp;
  Adafruit_Sensor *bmp_pressure;
  Adafruit_Sensor *bmp_temp;
};

#endif
